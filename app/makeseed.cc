#include <cstring>
#include <iostream>
#include <vector>

#include "arraylength.h"
#include "items.h"
#include "locations.h"
#include "mt_rand.h"
#include "world.h"

using namespace std;

void set_medallions(World &world) {
  const Item medallions[] = {Item::Ether, Item::Bombos, Item::Quake};
  world.set_medallion(Location::MiseryMireMedallion, medallions[mt_rand(0, 2)]);
  world.set_medallion(Location::TurtleRockMedallion, medallions[mt_rand(0, 2)]);
}

void fill_prizes(World &world) {
  Item prizes[ARRAY_LENGTH(PRIZES)];
  memcpy(prizes, PRIZES, sizeof(PRIZES));

  mt_shuffle<Item>(prizes, ARRAY_LENGTH(prizes));
  for (uint i = 0; i < ARRAY_LENGTH(prizes); i++) {
    // The PHP pops from the end of its shuffled array of prizes.
    world.set_item(PRIZE_LOCATIONS[i], prizes[ARRAY_LENGTH(PRIZES) - i - 1]);
    cout << LOCATION_NAMES[(int)PRIZE_LOCATIONS[i]]
         << " := " << ITEM_NAMES[(int)prizes[ARRAY_LENGTH(PRIZES) - i - 1]]
         << endl;
  }
}

void fill_items_in_locations(World &world, const Item *items,
                             Location *locations) {
  for (const Item *i = items; *i != Item::INVALID; i++) {
    for (Location *l = locations; *l != Location::INVALID; l++) {
      if (!world.has_item(*l) && world.can_fill(*l, *i) &&
          world.can_reach_with_one_fewer_item(*l, *i)) {
        world.set_item(*l, *i);
        break;
      }
    }
  }
}

void fast_fill_items_in_locations(World &world, const Item *items, size_t n,
                                  Location *locations) {
  Location *next = locations - 1;
  for (uint i = 0; i < n; i++) {
    while (next++, *next != Location::INVALID && world.has_item(*next))
      ;
    if (*next == Location::INVALID) {
      world.print();
      cout << "Ran out of locations." << endl;
      assert(false);
    }
    world.set_item(*next, items[i]);
  }
}

World makeseed(int seed) {
  World world;
  php_srand(seed);

  set_medallions(world);
  fill_prizes(world);

  // Create a list of the _valid_ locations (starting at 1).
  // locations.
  Location locations[(int)Location::NUM_LOCATIONS];
  for (uint i = 1; i < (int)Location::NUM_LOCATIONS; i++) {
    locations[i - 1] = (Location)i;
  }
  // Null terminate the list.
  locations[(int)Location::NUM_LOCATIONS - 1] = Location::INVALID;

  // Shuffle the real locations (not the null terminator).
  mt_shuffle(locations, (size_t)Location::NUM_LOCATIONS - 1);

  // Fill dungeon items.  Fill the items into each dungeon separately, since
  // dungeon items can only go in their respective dungeons.
  for (Region dungeon = Region::HyruleCastleEscape;
       dungeon <= Region::GanonsTower; ((int &)dungeon)++) {
    // Pull the locations from the shuffled master list into a list in the
    // same order, but existing consecutively in memory so it can be passed to
    // a fill_items_in_locations just for the one dungeon.
    Location shuffled_dungeon_locations[MAX_DUNGEON_LOCATIONS + 1];
    memset(shuffled_dungeon_locations, 0, sizeof(shuffled_dungeon_locations));
    int found = 0;
    for (uint i = 0; i < ARRAY_LENGTH(locations); i++) {
      for (uint c = 0; DUNGEON_LOCATIONS[(int)dungeon][c] != Location::INVALID;
           c++) {
        if (locations[i] == DUNGEON_LOCATIONS[(int)dungeon][c]) {
          shuffled_dungeon_locations[found++] = locations[i];
          break;
        }
      }
    }

    // Fill the items passing just shuffled_dungeon_locations.
    fill_items_in_locations(world, DUNGEON_ITEMS[(int)dungeon],
                            shuffled_dungeon_locations);
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
  Location empty_locations[(int)Location::NUM_LOCATIONS + 1];
  int num_empty = 0;
  for (uint locations_offset = (int)Location::NUM_LOCATIONS - 2;
       locations_offset > 0; locations_offset--) {
    if (!world.has_item(locations[locations_offset])) {
      empty_locations[num_empty++] = locations[locations_offset];
    }
  }

  // Null terminate empty_locations.
  empty_locations[num_empty] = Location::INVALID;

  // Shuffle the advancement items.
  int num_advancement = ARRAY_LENGTH(ADVANCEMENT_ITEMS);
  Item advancement[num_advancement + 1];
  memcpy(advancement, ADVANCEMENT_ITEMS, sizeof(advancement));
  advancement[num_advancement] = Item::INVALID;
  mt_shuffle(advancement, num_advancement);

  // Fill advancement.
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
  Item nice[ARRAY_LENGTH(NICE_ITEMS)];
  memcpy(nice, NICE_ITEMS, sizeof(nice));
  fast_fill_items_in_locations(world, nice, ARRAY_LENGTH(nice),
                               empty_locations);

  // fast_fill_items_in_locations the shuffled remaining trash items (the
  // first `gt_junk` of them were already placed in Ganon's Tower).
  Item *trash = extra + gt_junk;
  fast_fill_items_in_locations(world, trash, ARRAY_LENGTH(extra) - gt_junk,
                               empty_locations);
  return world;
}

int main(int argc, char **argv) {
  count_items();

  int seed = 1;
  World result = makeseed(seed);
  result.print();
}
