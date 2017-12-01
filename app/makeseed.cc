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
  for (uint i = 0; i < n; i++) {
    if (!world.has_item(*locations)) {
      world.set_item(*locations, items[i]);
      locations++;
    }
  }
}

void makeseed(int seed) {
  World world;
  php_srand(seed);

  set_medallions(world);
  fill_prizes(world);

  // Shuffle locations.
  Location locations[(int)Location::NUM_LOCATIONS];
  for (uint i = 0; i < (int)Location::NUM_LOCATIONS; i++) {
    locations[i] = (Location)i;
  }
  mt_shuffle(locations + 1, (size_t)Location::NUM_LOCATIONS - 1);

  // Fill dungeon items.
  fill_items_in_locations(world, DUNGEON_ITEMS, ARRAY_LENGTH(DUNGEON_ITEMS),
                          locations);

  // Random junk fill in Ganon's tower.
  Location ganons_tower[ARRAY_LENGTH(GANONS_TOWER)];
  size_t num_empty_gt_locations = 0;
  for (uint i = 0; i < ARRAY_LENGTH(GANONS_TOWER); i++) {
    if (!world.has_item(GANONS_TOWER[i])) {
      ganons_tower[num_empty_gt_locations++] = GANONS_TOWER[i];
    }
  }
  int gt_junk = mt_rand(0, 15);
  vector<Location> gt_locations =
      mt_sample(ganons_tower, num_empty_gt_locations, gt_junk);

  Item extra[ARRAY_LENGTH(TRASH_ITEMS)];
  memcpy(extra, TRASH_ITEMS, sizeof(extra));
  mt_shuffle(extra, ARRAY_LENGTH(extra));
  fast_fill_items_in_locations(world, extra, ARRAY_LENGTH(extra),
                               gt_locations.data());
  // Proceeding from the back of locations, take the empty ones.  Shuffle the
  // advancement items. fiil advancement items into the reversed locations.
  // Filter down to the empty locations again. Shuffle them again, then ffiil
  // nice, and ffiil trash. Done.

  // We need to follow the exact procedure from the PHP randomizer if we want to
  // faithfully generate the exact layouts from the given seed value.  When we
  // switch to just generating seeds as fast as possible, we can change this to
  // be more memory-friendly.
}

int main(int argc, char **argv) {
  count_items();

  int seed = 1;
  makeseed(seed);
}
