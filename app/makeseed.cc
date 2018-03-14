#include <stdio.h>
#include <condition_variable>
#include <cstring>
#include <iostream>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "arraylength.h"
#include "errors.h"
#include "items.h"
#include "locations.h"
#include "spdlog/spdlog.h"
#include "sqlite3.h"
#include "world.h"

using namespace std;

#define PRODUCER_THREADS (12)
#define TRANSACTION_SIZE (1000)
#define MAX_QUEUE_LENGTH 10000

queue<int> work;
mutex m_work, m_cout;
shared_mutex m_shuffle;
int stop;
bool done = false;

queue<pair<int, int>> shuffle_stage[(int)Location::NUM_LOCATIONS];
condition_variable consumer_waiting[(int)Location::NUM_LOCATIONS];
mutex channel_mutex[(int)Location::NUM_LOCATIONS];

void consumer(int location) {
  sqlite3 *conn;

  char filename[80];
  sprintf(filename, "results/location-%03d.db", location);

  int status = sqlite3_open(filename, &conn);
  if (status != SQLITE_OK) {
    cerr << "Cannot open DB." << endl;
    exit(status);
  }

  status = sqlite3_exec(conn, "PRAGMA journal_mode = OFF;", nullptr, nullptr,
                        nullptr);
  if (status != SQLITE_OK) {
    cerr << "Bad pragma." << endl;
    exit(status);
  }

  sqlite3_stmt *begin, *commit;
  sqlite3_prepare_v2(conn, "BEGIN;", 16, &begin, nullptr);
  sqlite3_prepare_v2(conn, "COMMIT;", 16, &commit, nullptr);

  sqlite3_stmt *stmt[(int)Item::NUM_ITEMS];

  char buffer[256];
  for (int item = 1; item < (int)Item::NUM_ITEMS; item++) {
    sprintf(buffer, "INSERT OR REPLACE INTO item%03d VALUES (?);", item);
    sqlite3_prepare_v2(conn, buffer, 256, &stmt[item], nullptr);
  }

  while (!done) {
    sqlite3_reset(begin);
    sqlite3_reset(commit);

    while (shuffle_stage[location].empty() && !done) {
      unique_lock<mutex> channel_lock(channel_mutex[location]);
      consumer_waiting[location].wait(channel_lock);
    }
    sqlite3_step(begin);
    shared_lock<shared_mutex> lock(m_shuffle);
    while (!shuffle_stage[location].empty()) {
      int item = shuffle_stage[location].front().first;
      int seed = shuffle_stage[location].front().second;

      sqlite3_reset(stmt[item]);
      sqlite3_bind_int(stmt[item], 1, seed);
      sqlite3_step(stmt[item]);
      shuffle_stage[location].pop();
    }
    sqlite3_step(commit);
  }

  // Clean up.
  sqlite3_finalize(begin);
  sqlite3_finalize(commit);
  for (int item = 1; item < (int)Item::NUM_ITEMS; item++) {
    sqlite3_finalize(stmt[item]);
  }
  sqlite3_close(conn);
}

void producer() {
  int base;
  while (true) {
    {
      lock_guard<mutex> lock(m_work);
      if (work.empty()) {
        break;
      }
      base = work.front();
      work.pop();
    }
    {
      lock_guard<mutex> lock(m_cout);
      cout << "Working on " << base << "-"
           << min(base + TRANSACTION_SIZE, stop) - 1 << endl;
    }

    World *result[TRANSACTION_SIZE];
    for (int offset = 0; offset < TRANSACTION_SIZE && base + offset < stop;
         offset++) {
      try {
        result[offset] = new World(base + offset);
      } catch (CannotPlaceItem &e) {
        exit(1);
      }
    }

    {
      lock_guard<shared_mutex> lock(m_shuffle);
      for (int offset = 0; offset < TRANSACTION_SIZE && base + offset < stop;
           offset++) {
        auto assignments = result[offset]->view_assignments();
        for (int location = 1; location < (int)Location::NUM_LOCATIONS;
             location++) {
          int item = (int)assignments[location];
          shuffle_stage[location].push(make_pair(item, base + offset));
          consumer_waiting[location].notify_one();
        }
        delete result[offset];
      }
    }

    {
      lock_guard<mutex> lock(m_cout);
      cout << "Shipped " << base << "-"
           << min(base + TRANSACTION_SIZE, stop) - 1 << endl;
    }
  }

  {
    lock_guard<mutex> lock(m_cout);
    cout << "No more work. Shutting down." << endl;
  }
}

int main(int argc, char **argv) {
  spdlog::stdout_color_mt("trace");
  if (argc == 2) {
    int seed = atoi(argv[1]);
    try {
      World result(seed);
      result.print();
    } catch (CannotPlaceItem &e) {
      return 1;
    }
    return 0;
  }

  if (argc != 3) {
    return 2;
  }

  int start = 1;
  stop = 50001;
  // Inclusive ranges for the command line. Convert to inclusive-exclusive as
  // soon as possible.
  start = atoi(argv[1]);
  stop = atoi(argv[2]) + 1;

  thread producers[PRODUCER_THREADS];
  thread writers[(int)Location::NUM_LOCATIONS];

  for (int block = start; block < stop; block += TRANSACTION_SIZE) {
    work.push(block);
  }

  for (int t = 0; t < PRODUCER_THREADS; t++) {
    producers[t] = thread(producer);
  }
  for (int t = 1; t < (int)Location::NUM_LOCATIONS; t++) {
    writers[t] = thread(consumer, t);
  }

  for (int t = 0; t < PRODUCER_THREADS; t++) {
    producers[t].join();
  }
  done = true;
  for (int t = 1; t < (int)Location::NUM_LOCATIONS; t++) {
    consumer_waiting[t].notify_all();
    writers[t].join();
  }

  return 0;
}
