#include <iostream>
#include <vector>

#include "items.h"
#include "locations.h"
#include "mt_rand.h"
#include "world.h"

#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))

using namespace std;

void set_medallions(World &world) {
  const Item medallions[] = {Item::Ether, Item::Bombos, Item::Quake};
  world.set_item(Location::MiseryMireMedallion, medallions[mt_rand(0, 2)]);
  world.set_item(Location::TurtleRockMedallion, medallions[mt_rand(0, 2)]);
}

void fill_prizes(World &world) {
  Item prizes[] = {
      Item::Crystal1,        Item::Crystal2,         Item::Crystal3,
      Item::Crystal4,        Item::Crystal5,         Item::Crystal6,
      Item::Crystal7,        Item::PendantOfCourage, Item::PendantOfPower,
      Item::PendantOfWisdom,
  };
  const Location prize_locations[] = {
      Location::EasternPalacePrize, Location::DesertPalacePrize,
      Location::TowerofHeraPrize,   Location::PalaceofDarknessPrize,
      Location::SwampPalacePrize,   Location::SkullWoodsPrize,
      Location::ThievesTownPrize,   Location::IcePalacePrize,
      Location::MiseryMirePrize,    Location::TurtleRockPrize,
  };
  mt_shuffle(ARRAY_LENGTH(prizes), prizes);
  for (unsigned int i = 0; i < ARRAY_LENGTH(prize_locations); i++) {
    world.set_item(prize_locations[i], prizes[i]);
  }
}

void makeseed(int seed) {
  cout << seed << endl;
  World world;
  set_medallions(world);
  fill_prizes(world);

  // Fill dungeon items. Assume we have all progression items.
}

int main(int argc, char **argv) {
  int seed = 1;
  makeseed(seed);
}
