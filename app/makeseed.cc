#include <cstring>
#include <iostream>
#include <vector>

#include "PHP_mt19937.h"
#include "arraylength.h"
#include "items.h"
#include "locations.h"
#include "mt_rand.h"
#include "world.h"

using namespace std;

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

// Returns the implication:
//(`item` is a dungeon item) -> (`location` is in the `item`'s dungeon)
bool dungeon_item_in_dungeon_location(Item item, Location location) {
  for (Region r = Region::HyruleCastleEscape; r <= Region::GanonsTower;
       ((int &)r)++) {
    for (const Item *dungeon_item = DUNGEON_ITEMS[(int)r];
         *dungeon_item != Item::INVALID; dungeon_item++) {
      if (item == *dungeon_item) {
        // `item` is a dungeon item belonging to dungeon `r`. Return (`location`
        // in dungeon `r`).
        for (const Location *dungeon_location = DUNGEON_LOCATIONS[(int)r];
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

void set_medallions(World &world) {
  const Item medallions[] = {Item::Ether, Item::Bombos, Item::Quake};
  world.set_medallion(Location::MiseryMireMedallion, medallions[mt_rand(0, 2)]);
  world.set_medallion(Location::TurtleRockMedallion, medallions[mt_rand(0, 2)]);
}

void fill_prizes(World &world) {
  Item prizes[ARRAY_LENGTH(PRIZES)];
  memcpy(prizes, PRIZES, sizeof(PRIZES));

  mt_shuffle<Item>(prizes, ARRAY_LENGTH(prizes));

  // PHP: There are 10 extra calls to getAdvancementItems->...->getBottle calls
  // that are wasted.
  for (int i = 0; i < 10; i++) {
    get_bottle(0);
  }
  // Then two more for setting fountain prizes that we don't care about.
  for (int i = 0; i < 2; i++) {
    get_bottle(1);
  }

  for (uint i = 0; i < ARRAY_LENGTH(prizes); i++) {
    // The PHP pops from the end of its shuffled array of prizes.
    world.set_item(PRIZE_LOCATIONS[i], prizes[ARRAY_LENGTH(PRIZES) - i - 1]);
  }
}

void fill_items_in_locations(World &world, const Item *items,
                             Location *locations) {
  for (const Item *i = items; *i != Item::INVALID; i++) {
    for (Location *l = locations; *l != Location::INVALID; l++) {
      if (!world.has_item(*l) && world.can_fill(*l, *i) &&
          world.can_reach_with_one_fewer_item(*l, *i) &&
          dungeon_item_in_dungeon_location(*i, *l)) {
        world.set_item(*l, *i);
        break;
      }
    }
  }
}

void fast_fill_items_in_locations(World &world, const Item *items, size_t n,
                                  Location *locations) {
  Location *next = locations;
  for (const Item *item_to_place = items + n - 1; item_to_place >= items;
       item_to_place--) {
    while (*next != Location::INVALID && world.has_item(*next)) {
      next++;
    }
    if (*next == Location::INVALID) {
      world.print();
      cerr << "Ran out of locations." << endl;
      assert(false);
    }
    world.set_item(*next, *item_to_place);
  }
}

World makeseed(int seed) {
  World world;
  mt_srand(seed);

  set_medallions(world);
  fill_prizes(world);

  // Initialize the advancement items here.  Bottle contents are randomized and
  // all random calls have to match PHP exactly, so determine the contents here.
  int num_advancement = ARRAY_LENGTH(ADVANCEMENT_ITEMS);
  Item advancement[num_advancement + 1];
  memcpy(advancement, ADVANCEMENT_ITEMS, sizeof(advancement));
  advancement[num_advancement] = Item::INVALID;
  for (int i = 0; i < num_advancement; i++) {
    if (advancement[i] == Item::Bottle) {
      advancement[i] = get_bottle(0);
    }
  }

  // Similarly for bottles in nice items.
  Item nice[ARRAY_LENGTH(NICE_ITEMS)];
  memcpy(nice, NICE_ITEMS, sizeof(nice));
  for (uint i = 0; i < ARRAY_LENGTH(nice); i++) {
    if (nice[i] == Item::Bottle) {
      nice[i] = get_bottle(0);
    }
  }

  // Copy the fillable locations to a local array so we can shuffle it.
  Location locations[NUM_FILLABLE_LOCATIONS + 1];
  memcpy(locations, FILLABLE_LOCATIONS,
         NUM_FILLABLE_LOCATIONS * sizeof(Location));
  // Null terminate the list.
  locations[NUM_FILLABLE_LOCATIONS] = Location::INVALID;

  // Shuffle the real locations (not the null terminator).
  mt_shuffle(locations, NUM_FILLABLE_LOCATIONS);

  // Fill dungeon items.  Fill the items into each dungeon separately, since
  // dungeon items can only go in their respective dungeons.
  fill_items_in_locations(world, FLAT_DUNGEON_ITEMS, locations);

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

  // Shuffle the advancement items.
  mt_shuffle(advancement, num_advancement);

  // Fill advancement.
  fill_items_in_locations(world, advancement, empty_locations);

  // Filter down to the empty locations again.
  num_empty = 0;
  for (int i = 0; empty_locations[i] != Location::INVALID; i++) {
    if (!world.has_item(empty_locations[i])) {
      empty_locations[num_empty++] = empty_locations[i];
      // cout << LOCATION_NAMES[(int)empty_locations[num_empty]] << endl;
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

  return world;
}

int main(int argc, char **argv) {
  count_items();

  int seed = 2;
  mt_srand(seed);
  World result = makeseed(seed);
  // result.print();
}
