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
  mt_shuffle<Item>(ARRAY_LENGTH(prizes), prizes);
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
      }
    }
  }
}

void makeseed(int seed) {
  World world;
  set_medallions(world);
  fill_prizes(world);

  // Shuffle locations.
  Location locations[(int)Location::NUM_LOCATIONS];
  for (uint i = 0; i < (int)Location::NUM_LOCATIONS; i++) {
    locations[i] = (Location)i;
  }
  mt_shuffle((size_t)Location::NUM_LOCATIONS - 1, locations + 1);

  // Fill dungeon items.

  fill_items_in_locations(world, DUNGEON_ITEMS, ARRAY_LENGTH(DUNGEON_ITEMS),
                          locations);
  world.print();

  int gt_junk = mt_rand(0, 15);
  Location ganons_tower[] = {
      Location::GanonsTowerBigChest,
      Location::GanonsTowerBigKeyChest,
      Location::GanonsTowerBigKeyRoomLeft,
      Location::GanonsTowerBigKeyRoomRight,
      Location::GanonsTowerBobsChest,
      Location::GanonsTowerBobsTorch,
      Location::GanonsTowerCompassRoomBottomLeft,
      Location::GanonsTowerCompassRoomBottomRight,
      Location::GanonsTowerCompassRoomTopLeft,
      Location::GanonsTowerCompassRoomTopRight,
      Location::GanonsTowerDMsRoomBottomLeft,
      Location::GanonsTowerDMsRoomBottomRight,
      Location::GanonsTowerDMsRoomTopLeft,
      Location::GanonsTowerDMsRoomTopRight,
      Location::GanonsTowerFiresnakeRoom,
      Location::GanonsTowerHopeRoomLeft,
      Location::GanonsTowerHopeRoomRight,
      Location::GanonsTowerMapChest,
      Location::GanonsTowerMiniHelmasaurRoomLeft,
      Location::GanonsTowerMiniHelmasaurRoomRight,
      Location::GanonsTowerMoldormChest,
      Location::GanonsTowerPreMoldormChest,
      Location::GanonsTowerPrize,
      Location::GanonsTowerRandomizerRoomBottomLeft,
      Location::GanonsTowerRandomizerRoomBottomRight,
      Location::GanonsTowerRandomizerRoomTopLeft,
      Location::GanonsTowerRandomizerRoomTopRight,
      Location::GanonsTowerTileRoom,
  };
  vector<Location> sample =
      mt_sample(gt_junk, ARRAY_LENGTH(ganons_tower), ganons_tower);
  Item extra[ARRAY_LENGTH(TRASH_ITEMS)];
  memcpy(extra, TRASH_ITEMS, sizeof(TRASH_ITEMS));
  mt_shuffle(ARRAY_LENGTH(extra), extra);
  uint n = 0;
  for (auto l = sample.cbegin(); l != sample.cend(); l++) {
    world.set_item(*l, extra[n]);
  }
}

int main(int argc, char **argv) {
  count_items();

  int seed = 1;
  makeseed(seed);
}
