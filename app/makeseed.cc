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
#include "world.h"

using namespace std;

#define PRODUCER_THREADS (12)
#define TRANSACTION_SIZE (1000)

struct location_and_seed {
  Location location;
  int seed;
};

queue<int> work;
mutex m_work, m_cout;
shared_mutex m_shuffle;
int stop;
bool done = false;

queue<struct location_and_seed> shuffle_stage[(int)Item::NUM_ITEMS];
condition_variable consumer_waiting[(int)Item::NUM_ITEMS];
mutex channel_mutex[(int)Item::NUM_ITEMS];

void consumer(int item) {
  FILE *sink[(int)Location::NUM_LOCATIONS];
  {
    char filename[32];
    for (int location = 1; location < (int)Location::NUM_LOCATIONS;
         location++) {
      sprintf(filename, "results/item%03d-location%03d.bin", item, location);
      sink[location] = fopen(filename, "a");
      if (!sink[location]) {
        cerr << "Couldn't open " << filename << endl;
        exit(1);
      }
    }
  }

  while (!done) {
    while (shuffle_stage[item].empty() && !done) {
      unique_lock<mutex> channel_lock(channel_mutex[item]);
      consumer_waiting[item].wait(channel_lock);
    }
    shared_lock<shared_mutex> lock(m_shuffle);
    while (!shuffle_stage[item].empty()) {
      int location = (int)shuffle_stage[item].front().location;
      fwrite(&shuffle_stage[item].front().seed, sizeof(int), 1, sink[location]);
      shuffle_stage[item].pop();
    }
  }
  for (int location = 1; location < (int)Location::NUM_LOCATIONS; location++) {
    fclose(sink[location]);
  }
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
          struct location_and_seed temp;
          temp.location = (Location)location;
          temp.seed = base + offset;
          shuffle_stage[item].push(temp);
          consumer_waiting[item].notify_one();
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
  spdlog::stdout_color_mt("console");
  spdlog::set_pattern("%L%m%d %H:%M:%S.%f %v");
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
  thread consumers[(int)Item::NUM_ITEMS];

  for (int block = start; block < stop; block += TRANSACTION_SIZE) {
    work.push(block);
  }

  for (int t = 1; t < (int)Item::NUM_ITEMS; t++) {
    consumers[t] = thread(consumer, t);
  }
  for (int t = 0; t < PRODUCER_THREADS; t++) {
    producers[t] = thread(producer);
  }
  for (int t = 0; t < PRODUCER_THREADS; t++) {
    producers[t].join();
  }
  done = true;
  for (int t = 1; t < (int)Item::NUM_ITEMS; t++) {
    consumer_waiting[t].notify_all();
    consumers[t].join();
  }

  return 0;
}
