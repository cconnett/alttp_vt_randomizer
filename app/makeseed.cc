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

  const Location prize_locations[] = {
      Location::EasternPalacePrize, Location::DesertPalacePrize,
      Location::TowerofHeraPrize,   Location::PalaceofDarknessPrize,
      Location::SwampPalacePrize,   Location::SkullWoodsPrize,
      Location::ThievesTownPrize,   Location::IcePalacePrize,
      Location::MiseryMirePrize,    Location::TurtleRockPrize,
  };
  mt_shuffle<Item>(prizes, ARRAY_LENGTH(prizes));
  for (uint i = 0; i < ARRAY_LENGTH(prize_locations); i++) {
    world.set_item(prize_locations[i], prizes[i]);
  }
}

void fill_items_in_locations(World &world, const Item *items, size_t n,
                             Location *locations) {
  for (uint i = 0; i < n; i++) {
    for (auto l = locations; l < (locations + (uint)Location::NUM_LOCATIONS);
         l++) {
      if (!world.has_item(*l) &&
          world.can_reach_with_one_fewer_item(*l, items[i])) {
        world.set_item(*l, items[i]);
        break;
      }
    }
  }
}

void fast_fill_items_in_locations(World &world, const Item *items, size_t n,
                                  Location *locations) {
  Location *next = locations - 1;
  for (uint i = 0; i < n; i++) {
    while (world.has_item(*++next))
      ;
    world.set_item(*next, items[i]);
  }
}

World makeseed(int seed) {
  World world;
  world.print();
  getchar();
  php_srand(seed);

  set_medallions(world);
  world.print();
  getchar();
  fill_prizes(world);

  // Shuffle locations.
  Location locations[(int)Location::NUM_LOCATIONS];
  for (uint i = 1; i < (int)Location::NUM_LOCATIONS; i++) {
    locations[i] = (Location)i;
  }
  mt_shuffle(locations + 1, (size_t)Location::NUM_LOCATIONS - 1);

  world.print();
  getchar();
  // Fill dungeon items.
  fill_items_in_locations(world, DUNGEON_ITEMS, ARRAY_LENGTH(DUNGEON_ITEMS),
                          locations + 1);

  // Random junk fill in Ganon's tower.
  Location ganons_tower_empty[ARRAY_LENGTH(GANONS_TOWER)];
  size_t num_empty_gt_locations = 0;
  for (uint i = 0; i < ARRAY_LENGTH(GANONS_TOWER); i++) {
    if (!world.has_item(GANONS_TOWER[i])) {
      ganons_tower_empty[num_empty_gt_locations++] = GANONS_TOWER[i];
    }
  }
  int gt_junk = mt_rand(0, 15);
  vector<Location> gt_trash_locations =
      mt_sample(ganons_tower_empty, num_empty_gt_locations, gt_junk);

  Item extra[ARRAY_LENGTH(TRASH_ITEMS)];
  memcpy(extra, TRASH_ITEMS, sizeof(extra));
  mt_shuffle(extra, ARRAY_LENGTH(extra));
  world.print();
  getchar();
  fast_fill_items_in_locations(world, extra, gt_trash_locations.size(),
                               gt_trash_locations.data());

  // 1. Proceeding from the back of locations and take the empty ones.
  Location empty_locations[(int)Location::NUM_LOCATIONS];
  for (int num_empty = 0, locations_offset = (int)Location::NUM_LOCATIONS - 1;
       locations_offset > 0; locations_offset--) {
    if (!world.has_item(locations[locations_offset])) {
      empty_locations[num_empty++] = locations[locations_offset];
    }
  }
  // 2. Shuffle the advancement items.
  Item advancement[ARRAY_LENGTH(ADVANCEMENT_ITEMS)];
  memcpy(advancement, ADVANCEMENT_ITEMS, sizeof(advancement));
  mt_shuffle(advancement, ARRAY_LENGTH(advancement));
  // 3. `fiil` advancement items into the reversed locations.

  world.print();
  getchar();
  fill_items_in_locations(world, advancement, ARRAY_LENGTH(advancement),
                          empty_locations);
  // 4. Filter down to the empty locations again.
  memset(empty_locations, 0, sizeof(empty_locations));
  int num_empty = 0;
  for (int locations_offset = (int)Location::NUM_LOCATIONS - 1;
       locations_offset > 0; locations_offset--) {
    if (!world.has_item(locations[locations_offset])) {
      empty_locations[num_empty++] = locations[locations_offset];
    }
  }
  // 5. Shuffle them again.
  mt_shuffle(empty_locations, num_empty);

  // 6. `ffiil` shuffled nice
  Item nice[ARRAY_LENGTH(NICE_ITEMS)];
  memcpy(nice, NICE_ITEMS, sizeof(nice));
  world.print();
  getchar();
  fast_fill_items_in_locations(world, nice, ARRAY_LENGTH(nice),
                               empty_locations);

  // 7. `ffiil` shuffled trash
  Item *trash = extra + gt_junk;
  world.print();
  getchar();
  fast_fill_items_in_locations(world, trash, ARRAY_LENGTH(extra) - gt_junk,
                               empty_locations);
  world.print();

  // We need to follow the exact procedure from the PHP randomizer if we want to
  // faithfully generate the exact layouts from the given seed value.  When we
  // switch to just generating seeds as fast as possible, we can change this to
  // be more memory-friendly.
  return world;
}

int main(int argc, char **argv) {
  count_items();

  int seed = 1;
  World result = makeseed(seed);
  result.print();
}
