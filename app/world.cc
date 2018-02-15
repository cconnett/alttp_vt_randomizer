#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "arraylength.h"
#include "errors.h"
#include "items.h"
#include "locations.h"
#include "sqlite3.h"
#include "world.h"

using namespace std;

// Returns the implication:
//(`item` is a dungeon item) -> (`location` is in the `item`'s dungeon)
bool dungeon_item_in_dungeon_location(Item item, Location location) {
  for (int r = (int)Region::HyruleCastleEscape; r <= (int)Region::GanonsTower;
       r++) {
    for (const Item *dungeon_item = DUNGEON_ITEMS[r];
         *dungeon_item != Item::INVALID; dungeon_item++) {
      if (item == *dungeon_item) {
        // `item` is a dungeon item belonging to dungeon `r`. Return (`location`
        // in dungeon `r`).
        for (const Location *dungeon_location = DUNGEON_LOCATIONS[r];
             *dungeon_location != Location::INVALID; dungeon_location++) {
          if (location == *dungeon_location) {
            // Item belongs in dungeon `r`, and `location` is in dungeon `r`.
            return true;
          }
        }
        // Item belongs in dungeon `r`, but `location` is not in dungeon `r`.
        return false;
      }
    }
    // Item doesn't belong to dungeon `r`.
  }
  // Item doesn't belong to any dungeon.
  return true;
}

World::World() {
  clear_assumed();
  clear_reachability_cache();
  memset(assignments, 0, sizeof(assignments));
  memset(constraints, 0, sizeof(constraints));

  raw_set_item(Location::HyruleCastleTowerPrize, Item::DefeatAgahnim);
  raw_set_item(Location::GanonsTowerPrize, Item::DefeatAgahnim2);
  raw_set_item(Location::DarkWorldNorthEastPrize, Item::DefeatGanon);
  raw_set_item(Location::SkullWoodsPinballRoom, Item::KeyD3);
}

void World::clear_reachability_cache() {
  memset(reachability_cache, 0, sizeof(reachability_cache));
}

void World::compact_print() {
  int col = 0;
  for (uint i = 1; i < ARRAY_LENGTH(assignments); i++) {
    cout << setw(4) << (int)assignments[i];
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

void World::print() {
  for (uint i = 1; i < ARRAY_LENGTH(assignments); i++) {
    cout << LOCATION_NAMES[i] << " = " << ITEM_NAMES[(int)assignments[i]]
         << endl;
  }
}

void World::sqlite3_write(sqlite3_stmt *stmt, int seed) {
  for (uint i = 1; i < ARRAY_LENGTH(assignments); i++) {
    if (assignments[i] != Item::INVALID) {
      sqlite3_reset(stmt);
      sqlite3_bind_int(stmt, 1, seed);
      sqlite3_bind_int(stmt, 2, i);
      sqlite3_bind_int(stmt, 3, (int)assignments[i]);
      sqlite3_step(stmt);
    }
  }
}

bool World::has_item(Location location) {
  assert(location != Location::INVALID);
  assert(location != Location::NUM_LOCATIONS);
  return (bool)assignments[(int)location];
}

void World::set_item(Location location, Item item) {
  raw_set_item(location, item);
  num_unplaced[(int)item]--;
  clear_reachability_cache();
}

bool World::check_and_set_item(Location location, Item item) {
  if (has_item(location)) {
    return false;
  }
  if (always_allow(location, item) ||
      (dungeon_item_in_dungeon_location(item, location) &&
       can_fill(location, item) && can_reach(location))) {
    incr_assumed(item);
    set_item(location, item);
    return true;
  }
  return false;
}

void World::raw_set_item(Location location, Item item) {
  assert(location != Location::INVALID);
  assert(location != Location::NUM_LOCATIONS);
  assert(item != Item::INVALID);
  assert(item != Item::NUM_ITEMS);
  assignments[(int)location] = item;
  where_is[(int)item].push_back(location);
#ifndef NDEBUG
  cout << LOCATION_NAMES[(int)location] << " := " << ITEM_NAMES[(int)item]
       << endl;
#endif
}

void World::clear_assumed() { memset(num_unplaced, 0, sizeof(num_unplaced)); }
void World::add_assumed(const Item *items, size_t n_items) {
  for (size_t i = 0; i < n_items; i++) {
    num_unplaced[(int)items[i]]++;
  }
}

void World::set_medallion(Location location, Item item) {
  assert(location != Location::INVALID);
  assert(location != Location::NUM_LOCATIONS);
  assert(item != Item::INVALID);
  assert(item != Item::NUM_ITEMS);
  assignments[(int)location] = item;
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

int World::bottle_count() {
  return num_reachable(Item::Bottle) + num_reachable(Item::BottleWithBee) +
         num_reachable(Item::BottleWithBluePotion) +
         num_reachable(Item::BottleWithFairy) +
         num_reachable(Item::BottleWithGoldBee) +
         num_reachable(Item::BottleWithGreenPotion) +
         num_reachable(Item::BottleWithRedPotion);
}
