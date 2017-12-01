#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "arraylength.h"
#include "items.h"
#include "locations.h"
#include "world.h"

using namespace std;

World::World() {
  memset(reachability_cache, 0, sizeof(reachability_cache));
  memset(assignments, 0, sizeof(assignments));
  memcpy(num_unplaced, INITIAL_UNPLACED, sizeof(INITIAL_UNPLACED));
}

void World::print() {
  int col = 0;
  for (uint i = 1; i < ARRAY_LENGTH(assignments); i++) {
    cout << setw(4) << (int)assignments[(int)i];
    if (++col >= 13) {
      cout << endl;
      col = 0;
    }
  }
  cout << endl;
  int num_empty = 0;
  for (uint i = 1; i < ARRAY_LENGTH(assignments); i++) {
    if (!has_item((Location)i)) {
      num_empty++;
    }
  }
  cout << "Empty: " << num_empty << endl;
}

bool World::has_item(Location location) {
  assert(location != Location::INVALID);
  assert(location != Location::NUM_LOCATIONS);
  return (bool)assignments[(int)location];
}

void World::set_item(Location location, Item item) {
  assert(location != Location::INVALID);
  assert(location != Location::NUM_LOCATIONS);
  assert(item != Item::INVALID);
  assert(item != Item::NUM_ITEMS);
  assignments[(int)location] = item;
  where_is[(int)item].push_back(location);
  num_unplaced[(int)item]--;
  memset(reachability_cache, 0, sizeof(reachability_cache));
}

void World::set_medallion(Location location, Item item) {
  assert(location != Location::INVALID);
  assert(location != Location::NUM_LOCATIONS);
  assert(item != Item::INVALID);
  assert(item != Item::NUM_ITEMS);
  assignments[(int)location] = item;
}

bool World::can_reach_with_one_fewer_item(Location location, Item item) {
  assert(location != Location::INVALID);
  assert(location != Location::NUM_LOCATIONS);
  assert(item != Item::INVALID);
  assert(item != Item::NUM_ITEMS);
  num_unplaced[(int)item]--;
  bool ret = can_reach(location);
  num_unplaced[(int)item]++;
  return ret;
}

bool World::can_reach(Location location) {
  assert(location != Location::INVALID);
  assert(location != Location::NUM_LOCATIONS);
  if (reachability_cache[(int)location]) {
    return reachability_cache[(int)location] > 0;
  }
  // Set it to -1 while we calculate reachability so that loops in the graph
  // see it as unreachable.
  reachability_cache[(int)location] = -1;
  reachability_cache[(int)location] = uncached_can_reach(location);
  return reachability_cache[(int)location];
}

int World::num_reachable(Item item) {
  assert(item != Item::INVALID);
  assert(item != Item::NUM_ITEMS);
  int count = num_unplaced[(int)item];
  for (auto i = where_is[(int)item].cbegin(); i != where_is[(int)item].cend();
       i++) {
    if (can_reach(*i)) {
      count += 1;
    }
  }
  return count;
}
