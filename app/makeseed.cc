#include <cstring>
#include <iostream>
#include <vector>

#include "arraylength.h"
#include "errors.h"
#include "items.h"
#include "locations.h"
#include "mt_rand.h"
#include "spdlog/spdlog.h"
#include "sqlite3.h"
#include "world.h"

using namespace std;

#define KEYSANITY false

Item get_bottle(int filled) {
  Item bottles[] = {
      Item::Bottle,
      Item::BottleWithRedPotion,
      Item::BottleWithGreenPotion,
      Item::BottleWithBluePotion,
      Item::BottleWithBee,
      Item::BottleWithGoldBee,
      Item::BottleWithFairy,
  };
  return bottles[mt_rand(filled, 6)];
}

void set_medallions(World &world) {
  const Item medallions[] = {Item::Ether, Item::Bombos, Item::Quake};
  world.set_medallion(Location::TurtleRockMedallion, medallions[mt_rand(0, 2)]);
  world.set_medallion(Location::MiseryMireMedallion, medallions[mt_rand(0, 2)]);
}

void fill_prizes(World &world) {
  Item prizes[ARRAY_LENGTH(PRIZES)];
  memcpy(prizes, PRIZES, sizeof(PRIZES));

  mt_shuffle<Item>(prizes, ARRAY_LENGTH(prizes));

  // PHP: There are 40 extra calls to getAdvancementItems->...->getBottle calls
  // that are wasted.
  for (int i = 0; i < 40; i++) {
    get_bottle(0);
  }
  // Then two more for setting fountain prizes that we don't care about.
  for (int i = 0; i < 2; i++) {
    get_bottle(1);
  }

  world.add_assumed(prizes, ARRAY_LENGTH(PRIZES));
  for (uint i = 0; i < ARRAY_LENGTH(prizes); i++) {
    // The PHP pops from the end of its shuffled array of prizes.
    world.set_item(PRIZE_LOCATIONS[i], prizes[ARRAY_LENGTH(PRIZES) - i - 1]);
  }
}

void fill_items_in_locations(World &world, const Item *items,
                             Location *locations) {
  auto log = spdlog::get("trace");
  for (const Item *i = items; *i != Item::INVALID; i++) {
    // Caution: The `assumed` count is decremented here, incremented in
    // check_and_set_item when it succeeds, and finally decremented again in
    // set_item.
    SPDLOG_TRACE(log, "Placing {}", ITEM_NAMES[(int)*i]);

    world.decr_assumed(*i);
    if (!world.constraints[(int)*i].empty()) {
      world.check_and_set_item(world.constraints[(int)*i].back(), *i);
      world.constraints[(int)*i].pop_back();
    } else {
      Location *l;
      for (l = locations; *l != Location::INVALID; l++) {
        if (world.check_and_set_item(*l, *i)) {
          break;
        }
      }
      if (*l == Location::INVALID) {
        cerr << "Can't place " << ITEM_NAMES[(int)*i] << endl;
        throw CannotPlaceItem();
      }
    }
  }
}

void fast_fill_items_in_locations(World &world, const Item *items, size_t n,
                                  Location *locations) {
  world.add_assumed(items, n);
  Location *next = locations;
  for (const Item *item_to_place = items + n - 1; item_to_place >= items;
       item_to_place--) {
    while (*next != Location::INVALID && world.has_item(*next)) {
      next++;
    }
    if (*next == Location::INVALID) {
      world.print();
      cerr << "Ran out of locations." << endl;
      throw CannotPlaceItem();
    }
    world.set_item(*next, *item_to_place);
  }
}

void makeseed(World &world, int seed) {
  mt_srand(seed);

  set_medallions(world);
  fill_prizes(world);

  // Initialize the advancement items here.  Bottle contents are randomized and
  // all random calls have to match PHP exactly, so determine the contents here.
  constexpr int num_advancement =
      ARRAY_LENGTH(ADVANCEMENT_ITEMS) +
      (KEYSANITY ? ARRAY_LENGTH(FLAT_DUNGEON_ITEMS) : 0);
  Item advancement[num_advancement + 1];
  memcpy(advancement, ADVANCEMENT_ITEMS, ARRAY_LENGTH(ADVANCEMENT_ITEMS));
  if (KEYSANITY) {
    memcpy(advancement + ARRAY_LENGTH(ADVANCEMENT_ITEMS), FLAT_DUNGEON_ITEMS,
           ARRAY_LENGTH(FLAT_DUNGEON_ITEMS));
  }
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
  mt_shuffle(advancement, num_advancement);

  // Copy the fillable locations to a local array so we can shuffle it.
  Location locations[NUM_FILLABLE_LOCATIONS + 1];
  memcpy(locations, FILLABLE_LOCATIONS,
         NUM_FILLABLE_LOCATIONS * sizeof(Location));
  // Null terminate the list.
  locations[NUM_FILLABLE_LOCATIONS] = Location::INVALID;

  // Shuffle the real locations (not the null terminator).
  mt_shuffle(locations, NUM_FILLABLE_LOCATIONS);

  if (!KEYSANITY) {
    // Fill dungeon items. The list of dungeon items is empty in keysanity;
    // those items are part of the advancement items.
    world.clear_assumed();
    world.add_assumed(FLAT_DUNGEON_ITEMS, ARRAY_LENGTH(FLAT_DUNGEON_ITEMS));
    world.add_assumed(ADVANCEMENT_ITEMS, ARRAY_LENGTH(ADVANCEMENT_ITEMS));
    fill_items_in_locations(world, FLAT_DUNGEON_ITEMS, locations);
  }

  // Random junk fill in Ganon's tower.
  Location ganons_tower_empty[MAX_DUNGEON_LOCATIONS];
  size_t num_empty_gt_locations = 0;
  for (uint i = 0;
       DUNGEON_LOCATIONS[(int)Region::GanonsTower][i] != Location::INVALID;
       i++) {
    if (!world.has_item(DUNGEON_LOCATIONS[(int)Region::GanonsTower][i])) {
      ganons_tower_empty[num_empty_gt_locations++] =
          DUNGEON_LOCATIONS[(int)Region::GanonsTower][i];
    }
  }
  int gt_junk = mt_rand(0, 15);
  vector<Location> gt_trash_locations =
      mt_sample(ganons_tower_empty, num_empty_gt_locations, gt_junk);

  Item extra[ARRAY_LENGTH(TRASH_ITEMS)];
  memcpy(extra, TRASH_ITEMS, sizeof(extra));
  mt_shuffle(extra, ARRAY_LENGTH(extra));
  fast_fill_items_in_locations(world, extra, gt_trash_locations.size(),
                               gt_trash_locations.data());

  // The dungeon items were filled into locations starting at the front of the
  // shuffled locations list. The PHP version then uses locations from the
  // back of the shuffled list for placing the advancement items. Proceeding
  // from the back of locations, take the empty ones into a new list.
  Location empty_locations[NUM_FILLABLE_LOCATIONS + 1];
  memset(empty_locations, 0, sizeof(empty_locations));
  int num_empty = 0;
  for (int offset = NUM_FILLABLE_LOCATIONS - 1; offset >= 0; offset--) {
    if (!world.has_item(locations[offset])) {
      empty_locations[num_empty++] = locations[offset];
    }
  }

  // Shuffle the advancement items. (Yes, again; the PHP does this)
  mt_shuffle(advancement, num_advancement);

  // Fill advancement.
  world.clear_assumed();
  world.add_assumed(advancement, ARRAY_LENGTH(advancement));
  fill_items_in_locations(world, advancement, empty_locations);

  // Filter down to the empty locations again.
  num_empty = 0;
  for (int i = 0; empty_locations[i] != Location::INVALID; i++) {
    if (!world.has_item(empty_locations[i])) {
      empty_locations[num_empty++] = empty_locations[i];
    }
  }
  // Null terminate empty_locations.
  empty_locations[num_empty] = Location::INVALID;
  // Shuffle them again.
  mt_shuffle(empty_locations, num_empty);

  // fast_fill_items_in_locations the shuffled nice items.
  mt_shuffle(nice, ARRAY_LENGTH(nice));
  fast_fill_items_in_locations(world, nice, ARRAY_LENGTH(nice),
                               empty_locations);

  // fast_fill_items_in_locations the shuffled remaining trash items. The first
  // `gt_junk` of them were already placed in Ganon's Tower, so offset by
  // `gt_junk`.
  Item *trash = extra + gt_junk;
  int num_trash = ARRAY_LENGTH(extra) - gt_junk;
  mt_shuffle(trash, num_trash);
  fast_fill_items_in_locations(world, trash, num_trash, empty_locations);
}

int main(int argc, char **argv) {
  spdlog::stdout_color_mt("trace");
  if (argc == 2) {
    int seed = atoi(argv[1]);
    World result;
    try {
      makeseed(result, seed);
    } catch (CannotPlaceItem &e) {
      return 1;
    }
    result.print();
    return 0;
  }
  uint begin = 1, end = 50000;
  if (argc == 3) {
    begin = atoi(argv[1]);
    end = atoi(argv[2]);
  }

  string filename = "seeds.db-";
  filename += argv[1];
  filename += "-";
  filename += argv[2];

  sqlite3 *conn;
  int status = sqlite3_open(filename.c_str(), &conn);
  if (status != SQLITE_OK) {
    return status;
  }
  sqlite3_exec(conn,
               "PRAGMA journal_mode = MEMORY;"
               "CREATE TABLE IF NOT EXISTS assignments "
               "(seed int, location int, item int);",
               nullptr, nullptr, nullptr);

  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(conn,
                     "INSERT OR REPLACE INTO assignments VALUES (?, ?, ?)", 256,
                     &stmt, nullptr);

  sqlite3_exec(conn, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
  for (uint seed = begin; seed < end; seed++) {
    World result;
    try {
      makeseed(result, seed);
    } catch (CannotPlaceItem &e) {
      return 1;
    }
    result.sqlite3_write(stmt, seed);

    if (seed % 1000 == 0) {
      sqlite3_exec(conn, "COMMIT TRANSACTION;", nullptr, nullptr, nullptr);
      cout << seed << endl;
      sqlite3_exec(conn, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    }
  }
  sqlite3_exec(conn, "COMMIT TRANSACTION;", nullptr, nullptr, nullptr);
  sqlite3_finalize(stmt);
  sqlite3_close(conn);
}
