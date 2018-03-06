#include <pthread.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
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

struct range {
  int begin;
  int end;
};

void *populate_seed_range(void *range_struct) {
  struct range myrange = *(struct range *)range_struct;
  cout << "Thread with range " << myrange.begin << "-" << myrange.end << endl;

  sqlite3 *conn;
  int status = sqlite3_open(DB_FILENAME, &conn);
  if (status != SQLITE_OK) {
    cerr << "Cannot open DB." << endl;
    exit(status);
  }

  sqlite3_stmt *stmt[(int)Item::NUM_ITEMS][(int)Location::NUM_LOCATIONS];
  char buffer[256];
  cout << "lo" << endl;
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
  cout << "hi" << endl;

  for (int base = myrange.begin; base < myrange.end; base += TRANSACTION_SIZE) {
    World *result[TRANSACTION_SIZE];
    for (int offset = 0;
         offset < TRANSACTION_SIZE && base + offset < myrange.end; offset++) {
      try {
        result[offset] = new World(base + offset);
      } catch (CannotPlaceItem &e) {
        exit(1);
      }
    }
    sqlite3_exec(conn, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    for (int offset = 0;
         offset < TRANSACTION_SIZE && base + offset < myrange.end; offset++) {
      result[offset]->sqlite3_write(stmt, base + offset);
      delete result[offset];
    }
    sqlite3_exec(conn, "COMMIT TRANSACTION;", nullptr, nullptr, nullptr);
    cout << "Completed " << base << "-"
         << min(base + TRANSACTION_SIZE - 1, myrange.end) << endl;
  }
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

  int begin = 1, end = 50000;
  begin = atoi(argv[1]);
  end = atoi(argv[2]);

  struct range ranges[THREADS];
  pthread_t threads[THREADS];

  for (int t = 0; t < THREADS; t++) {
    ranges[t].begin = begin + (end - begin) * t / THREADS;
    ranges[t].end = begin + (end - begin) * (t + 1) / THREADS;
    pthread_create(&threads[t], nullptr, populate_seed_range, &ranges[t]);
  }
  for (int t = 0; t < THREADS; t++) {
    pthread_join(threads[t], nullptr);
  }
  return 0;
}
