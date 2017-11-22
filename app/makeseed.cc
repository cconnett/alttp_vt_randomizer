#include <iostream>
#include <vector>

#include "items.h"
#include "locations.h"
#include "mt_rand.h"
#include "world.h"

#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))

using namespace std;

void set_medallions(World &world) {
  const int medallions[] = {Items::Ether, Items::Bombos, Items::Quake};
  world.set_item(Locations::MiseryMireMedallion, medallions[mt_rand(0, 2)]);
  world.set_item(Locations::TurtleRockMedallion, medallions[mt_rand(0, 2)]);
}

void fill_prizes(World &world) {
  int prizes[] = {
      Items::Crystal1,        Items::Crystal2,         Items::Crystal3,
      Items::Crystal4,        Items::Crystal5,         Items::Crystal6,
      Items::Crystal7,        Items::PendantOfCourage, Items::PendantOfPower,
      Items::PendantOfWisdom,
  };
  const int prize_locations[] = {
      Locations::EasternPalacePrize, Locations::DesertPalacePrize,
      Locations::TowerofHeraPrize,   Locations::PalaceofDarknessPrize,
      Locations::SwampPalacePrize,   Locations::SkullWoodsPrize,
      Locations::ThievesTownPrize,   Locations::IcePalacePrize,
      Locations::MiseryMirePrize,    Locations::TurtleRockPrize,
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
