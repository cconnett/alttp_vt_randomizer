#include <pthread.h>
#include <stdio.h>
#include <cstring>
#include <deque>
#include <iostream>
#include <mutex>
#include <vector>

#include "arraylength.h"
#include "errors.h"
#include "items.h"
#include "locations.h"
#include "spdlog/spdlog.h"
#include "sqlite3.h"
#include "world.h"

using namespace std;

#define THREADS (12)
#define TRANSACTION_SIZE (1000)
#define DB_FILENAME ("seeds.db")

deque<int> work;
mutex m_work;
int stop;

void *populate_seed_range(void *unused) {
  cout << "Warming up" << endl;

  sqlite3 *conn;
  int status = sqlite3_open(DB_FILENAME, &conn);
  if (status != SQLITE_OK) {
    cerr << "Cannot open DB." << endl;
    exit(status);
  }
  sqlite3_busy_timeout(conn, 180000);

  sqlite3_stmt *begin, *commit;
  sqlite3_prepare_v2(conn, "BEGIN;", 256, &begin, nullptr);
  sqlite3_prepare_v2(conn, "COMMIT;", 256, &commit, nullptr);

  sqlite3_stmt *stmt[(int)Item::NUM_ITEMS][(int)Location::NUM_LOCATIONS];

  char buffer[256];
  for (int item = 1; item < (int)Item::NUM_ITEMS; item++) {
    for (int location = 1; location < (int)Location::NUM_LOCATIONS;
         location++) {
      sprintf(buffer,
              "INSERT OR REPLACE INTO "
              "seeds_with_item%03d_at_location%03d VALUES (?);",
              item, location);
      sqlite3_prepare_v2(conn, buffer, 256, &stmt[item][location], nullptr);
    }
  }

  int base;
  while (true) {
    {
      lock_guard<mutex> lock(m_work);
      if (work.empty()) {
        break;
      }
      base = work.front();
      work.pop_front();
    }
    cout << "Working on " << base << "-" << min(base + TRANSACTION_SIZE, stop)
         << endl;

    World *result[TRANSACTION_SIZE];
    for (int offset = 0; offset < TRANSACTION_SIZE && base + offset < stop;
         offset++) {
      try {
        result[offset] = new World(base + offset);
      } catch (CannotPlaceItem &e) {
        exit(1);
      }
    }
    sqlite3_step(begin);
    for (int offset = 0; offset < TRANSACTION_SIZE && base + offset < stop;
         offset++) {
      result[offset]->sqlite3_write(stmt, base + offset);
      delete result[offset];
    }
    sqlite3_step(commit);
    sqlite3_reset(begin);
    sqlite3_reset(commit);

    cout << "Completed " << base << "-" << min(base + TRANSACTION_SIZE, stop)
         << endl;
  }

  cout << "No more work. Shutting down." << endl;
  sqlite3_close(conn);
  return nullptr;
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
  stop = 50000;
  start = atoi(argv[1]);
  stop = atoi(argv[2]);

  pthread_t threads[THREADS];

  for (int block = start; block < stop; block += TRANSACTION_SIZE) {
    work.push_back(block);
  }
  for (int t = 0; t < THREADS; t++) {
    pthread_create(&threads[t], nullptr, populate_seed_range, nullptr);
  }
  for (int t = 0; t < THREADS; t++) {
    pthread_join(threads[t], nullptr);
  }
  return 0;
}
