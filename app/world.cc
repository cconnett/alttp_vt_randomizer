#include <cassert>
#include <cstring>
#include <iomanip>
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

Region dungeon_of(Item item) {
  for (int r = (int)Region::HyruleCastleEscape; r <= (int)Region::GanonsTower;
       r++) {
    for (const Item *dungeon_item = DUNGEON_ITEMS[r];
         *dungeon_item != Item::INVALID; dungeon_item++) {
      if (item == *dungeon_item) {
        return (Region)r;
      }
    }
  }
  return Region::INVALID;
}

Region dungeon_of(Location location) {
  for (int r = (int)Region::HyruleCastleEscape; r <= (int)Region::GanonsTower;
       r++) {
    for (const Location *dungeon_location = DUNGEON_LOCATIONS[r];
         *dungeon_location != Location::INVALID; dungeon_location++) {
      if (location == *dungeon_location) {
        return (Region)r;
      }
    }
  }
  return Region::INVALID;
}

World::~World() {
  delete generator;
}

World::World(int seed) : generator(new mt_rand(seed)) {
  clear_assumed();
  clear_reachability_cache();
  memset(assignments, 0, sizeof(assignments));

  raw_set_item(Location::HyruleCastleTowerPrize, Item::DefeatAgahnim);
  raw_set_item(Location::GanonsTowerPrize, Item::DefeatAgahnim2);
  raw_set_item(Location::DarkWorldNorthEastPrize, Item::DefeatGanon);
  raw_set_item(Location::SkullWoodsPinballRoom, Item::KeyD3);

  set_medallions();
  fill_prizes();

  // Initialize the advancement items here.  Bottle contents are randomized and
  // all random calls have to match PHP exactly, so determine the contents here.
  int num_advancement = ARRAY_LENGTH(ADVANCEMENT_ITEMS);
  Item advancement[num_advancement + 1];
  memcpy(advancement, ADVANCEMENT_ITEMS, sizeof(advancement));
  advancement[num_advancement] = Item::INVALID;

  // Initialize nice items, too.
  Item nice[ARRAY_LENGTH(NICE_ITEMS)];
  memcpy(nice, NICE_ITEMS, sizeof(nice));

  // Assign actual bottle contents to the bottles in nice items and the one
  // bottle in advancement. The PHP generates four bottles, pulls them off the
  // advancement list, then puts the last one back on advancement and the other
  // three on nice items. To recreate this, generate contents for nice, then
  // advancement.
  for (uint i = 0; i < ARRAY_LENGTH(nice); i++) {
    if (nice[i] == Item::Bottle) {
      nice[i] = get_bottle(0);
    }
  }

  for (int i = 0; i < num_advancement; i++) {
    if (advancement[i] == Item::Bottle) {
      advancement[i] = get_bottle(0);
    }
  }

  // Shuffle the advancement items.
  generator->shuffle(advancement, num_advancement);

  // Copy the fillable locations to a local array so we can shuffle it.
  Location locations[NUM_FILLABLE_LOCATIONS + 1];
  memcpy(locations, FILLABLE_LOCATIONS,
         NUM_FILLABLE_LOCATIONS * sizeof(Location));
  // Null terminate the list.
  locations[NUM_FILLABLE_LOCATIONS] = Location::INVALID;

  // Shuffle the real locations (not the null terminator).
  generator->shuffle(locations, NUM_FILLABLE_LOCATIONS);

  // For each dungeon:
  //   Extract its locations from `locations`.
  // For each FLAT_DUNGEON_ITEMS:
  //   Apply fill_items_in_locations(world, single item,
  //   shuffled_locations[dungeon_of(item)])
  Location shuffled_locations_by_dungeon[NUM_DUNGEONS + 1]
                                        [MAX_DUNGEON_LOCATIONS + 1] = {
                                            Location::INVALID};
  Location *next_location_per_dungeon[NUM_DUNGEONS + 1];
  for (int d = (int)Region::HyruleCastleEscape; d <= (int)Region::GanonsTower;
       d++) {
    next_location_per_dungeon[d] = &shuffled_locations_by_dungeon[d][0];
  }
  for (Location *loc = locations; *loc != Location::INVALID; loc++) {
    Region dungeon = dungeon_of(*loc);
    if (dungeon != Region::INVALID) {
      *next_location_per_dungeon[(int)dungeon]++ = *loc;
    }
  }
  for (int d = (int)Region::HyruleCastleEscape; d <= (int)Region::GanonsTower;
       d++) {
    *next_location_per_dungeon[d] = Location::INVALID;
  }

  Item single_item[2] = {Item::INVALID};
  for (const Item *item = FLAT_DUNGEON_ITEMS; *item != Item::INVALID; item++) {
    single_item[0] = *item;
    fill_items_in_locations(
        single_item, shuffled_locations_by_dungeon[(int)dungeon_of(*item)]);
  }

  // Random junk fill in Ganon's tower.
  Location ganons_tower_empty[MAX_DUNGEON_LOCATIONS];
  size_t num_empty_gt_locations = 0;
  for (uint i = 0;
       DUNGEON_LOCATIONS[(int)Region::GanonsTower][i] != Location::INVALID;
       i++) {
    if (!has_item(DUNGEON_LOCATIONS[(int)Region::GanonsTower][i])) {
      ganons_tower_empty[num_empty_gt_locations++] =
          DUNGEON_LOCATIONS[(int)Region::GanonsTower][i];
    }
  }
  int gt_junk = generator->rand(0, 15);
  vector<Location> gt_trash_locations =
      generator->sample(ganons_tower_empty, num_empty_gt_locations, gt_junk);

  Item extra[ARRAY_LENGTH(TRASH_ITEMS)];
  memcpy(extra, TRASH_ITEMS, sizeof(extra));
  generator->shuffle(extra, ARRAY_LENGTH(extra));
  fast_fill_items_in_locations(extra, gt_trash_locations.size(),
                               gt_trash_locations.data());

  // The dungeon items were filled into locations starting at the front of the
  // shuffled locations list. The PHP version then uses locations from the
  // back of the shuffled list for placing the advancement items. Proceeding
  // from the back of locations, take the empty ones into a new list.
  Location empty_locations[NUM_FILLABLE_LOCATIONS + 1];
  memset(empty_locations, 0, sizeof(empty_locations));
  int num_empty = 0;
  for (int offset = NUM_FILLABLE_LOCATIONS - 1; offset >= 0; offset--) {
    if (!has_item(locations[offset])) {
      empty_locations[num_empty++] = locations[offset];
    }
  }

  // Shuffle the advancement items. (Yes, again; the PHP does this)
  generator->shuffle(advancement, num_advancement);

  // Fill advancement.
  clear_assumed();
  add_assumed(advancement, ARRAY_LENGTH(advancement));
  fill_items_in_locations(advancement, empty_locations);

  // Filter down to the empty locations again.
  num_empty = 0;
  for (int i = 0; empty_locations[i] != Location::INVALID; i++) {
    if (!has_item(empty_locations[i])) {
      empty_locations[num_empty++] = empty_locations[i];
    }
  }
  // Null terminate empty_locations.
  empty_locations[num_empty] = Location::INVALID;
  // Shuffle them again.
  generator->shuffle(empty_locations, num_empty);

  // fast_fill_items_in_locations the shuffled nice items.
  generator->shuffle(nice, ARRAY_LENGTH(nice));
  fast_fill_items_in_locations(nice, ARRAY_LENGTH(nice), empty_locations);

  // fast_fill_items_in_locations the shuffled remaining trash
  // items. The first `gt_junk` of them were already placed in Ganon's
  // Tower, so offset by `gt_junk`.
  Item *trash = extra + gt_junk;
  int num_trash = ARRAY_LENGTH(extra) - gt_junk;
  generator->shuffle(trash, num_trash);
  fast_fill_items_in_locations(trash, num_trash, empty_locations);
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

void World::sqlite3_write(sqlite3_stmt *stmt[][(int)Location::NUM_LOCATIONS],
                          int seed) {
  for (uint location = 1; location < ARRAY_LENGTH(assignments); location++) {
    if (assignments[location] != Item::INVALID) {
      auto s = stmt[(int)assignments[location]][location];
      sqlite3_reset(s);
      sqlite3_bind_int(s, 1, seed);
      sqlite3_step(s);
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

  SPDLOG_TRACE(log, "Trying {} for {}", LOCATION_NAMES[(int)location],
               ITEM_NAMES[(int)item]);
  if (always_allow(location, item)) {
    SPDLOG_TRACE(log, "{} always allowed in {}", ITEM_NAMES[(int)item],
                 LOCATION_NAMES[(int)location]);
  }

  if (always_allow(location, item) ||
      (can_fill(location, item) && can_reach(location))) {
    incr_assumed(item);
    set_item(location, item);
    return true;
  }
  SPDLOG_TRACE(log, "{} /= {}", LOCATION_NAMES[(int)location],
               ITEM_NAMES[(int)item]);
  return false;
}

void World::raw_set_item(Location location, Item item) {
  assert(location != Location::INVALID);
  assert(location != Location::NUM_LOCATIONS);
  assert(item != Item::INVALID);
  assert(item != Item::NUM_ITEMS);
  assignments[(int)location] = item;
  where_is[(int)item].push_back(location);
  SPDLOG_TRACE(log, "{} := {}", LOCATION_NAMES[(int)location],
               ITEM_NAMES[(int)item]);
}

void World::clear_assumed() {
  memset(num_unplaced, 0, sizeof(num_unplaced));
}
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
    SPDLOG_TRACE(log, "Cached. {}",
                 reachability_cache[(int)location] > 0 ? "Good." : "Bad.");
    return reachability_cache[(int)location] > 0;
  }
  // Set it to -1 while we calculate reachability so that loops in the graph
  // see it as unreachable.
  SPDLOG_TRACE(log, "Descending.");
  reachability_cache[(int)location] = -1;
  reachability_cache[(int)location] = uncached_can_reach(location);
  SPDLOG_TRACE(log, "Answer found: {} is{} reachable.",
               LOCATION_NAMES[(int)location],
               reachability_cache[(int)location] > 0 ? "" : " not");
  return reachability_cache[(int)location] > 0;
}

int World::num_reachable(Item item) {
  assert(item != Item::INVALID);
  assert(item != Item::NUM_ITEMS);

  int count = num_unplaced[(int)item];

  SPDLOG_TRACE(log, "Searching for {}. {} unplaced and assumed reachable.",
               ITEM_NAMES[(int)item], count);

  for (auto loc = where_is[(int)item].cbegin();
       loc != where_is[(int)item].cend(); loc++) {
    SPDLOG_TRACE(log, "Looking in {}", LOCATION_NAMES[(int)*loc]);
    if (can_reach(*loc)) {
      count += 1;
    }
  }
  SPDLOG_TRACE(log, "Found {} of {}", count, ITEM_NAMES[(int)item]);
  return count;
}

int World::is_num_reachable(int n, Item item) {
  assert(item != Item::INVALID);
  assert(item != Item::NUM_ITEMS);

  int count = num_unplaced[(int)item];
  if (count >= n) {
    return true;
  }

  SPDLOG_TRACE(log, "Searching for {}. {} unplaced and assumed reachable.",
               ITEM_NAMES[(int)item], count);

  for (auto loc = where_is[(int)item].cbegin();
       loc != where_is[(int)item].cend(); loc++) {
    SPDLOG_TRACE(log, "Looking in {}", LOCATION_NAMES[(int)*loc]);
    if (can_reach(*loc)) {
      count += 1;
      if (count >= n) {
        return true;
      }
    }
  }
  SPDLOG_TRACE(log, "Found {} of {}", count, ITEM_NAMES[(int)item]);
  return false;
}

int World::bottle_count() {
  return num_reachable(Item::Bottle) + num_reachable(Item::BottleWithBee) +
         num_reachable(Item::BottleWithBluePotion) +
         num_reachable(Item::BottleWithFairy) +
         num_reachable(Item::BottleWithGoldBee) +
         num_reachable(Item::BottleWithGreenPotion) +
         num_reachable(Item::BottleWithRedPotion);
}

Item World::get_bottle(int filled) {
  Item bottles[] = {
      Item::Bottle,
      Item::BottleWithRedPotion,
      Item::BottleWithGreenPotion,
      Item::BottleWithBluePotion,
      Item::BottleWithBee,
      Item::BottleWithGoldBee,
      Item::BottleWithFairy,
  };
  return bottles[generator->rand(filled, 6)];
}

void World::set_medallions() {
  const Item medallions[] = {Item::Ether, Item::Bombos, Item::Quake};
  set_medallion(Location::TurtleRockMedallion,
                medallions[generator->rand(0, 2)]);
  set_medallion(Location::MiseryMireMedallion,
                medallions[generator->rand(0, 2)]);
}

void World::fill_prizes() {
  Item prizes[ARRAY_LENGTH(PRIZES)];
  memcpy(prizes, PRIZES, sizeof(PRIZES));

  generator->shuffle<Item>(prizes, ARRAY_LENGTH(prizes));

  // PHP: There are 40 extra calls to getAdvancementItems->...->getBottle calls
  // that are wasted.
  for (int i = 0; i < 40; i++) {
    get_bottle(0);
  }
  // Then two more for setting fountain prizes that we don't care about.
  for (int i = 0; i < 2; i++) {
    get_bottle(1);
  }

  add_assumed(prizes, ARRAY_LENGTH(PRIZES));
  for (uint i = 0; i < ARRAY_LENGTH(prizes); i++) {
    // The PHP pops from the end of its shuffled array of prizes.
    set_item(PRIZE_LOCATIONS[i], prizes[ARRAY_LENGTH(PRIZES) - i - 1]);
  }
}

void World::fill_items_in_locations(const Item *items, Location *locations) {
  auto log = spdlog::get("trace");
  for (const Item *i = items; *i != Item::INVALID; i++) {
    // Caution: The `assumed` count is decremented here, incremented in
    // check_and_set_item when it succeeds, and finally decremented again in
    // set_item.
    SPDLOG_TRACE(log, "Placing {}", ITEM_NAMES[(int)*i]);

    decr_assumed(*i);
    Location *l;
    for (l = locations; *l != Location::INVALID; l++) {
      if (check_and_set_item(*l, *i)) {
        break;
      }
    }
    if (*l == Location::INVALID) {
      cerr << "Can't place " << ITEM_NAMES[(int)*i] << endl;
      throw CannotPlaceItem();
    }
  }
}

void World::fast_fill_items_in_locations(const Item *items, size_t n,
                                         Location *locations) {
  add_assumed(items, n);
  Location *next = locations;
  for (const Item *item_to_place = items + n - 1; item_to_place >= items;
       item_to_place--) {
    while (*next != Location::INVALID && has_item(*next)) {
      next++;
    }
    if (*next == Location::INVALID) {
      print();
      cerr << "Ran out of locations." << endl;
      throw CannotPlaceItem();
    }
    set_item(*next, *item_to_place);
  }
}
