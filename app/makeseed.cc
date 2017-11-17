#include <iostream>
#include <vector>

#include "items.h"
#include "locations.h"
#include "mt_rand.h"

#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))

using namespace std;

int assignments[Locations::NUM_LOCATIONS];

void setMedallions() {
  const int medallions[] = {Items::Ether, Items::Bombos, Items::Quake};
  assignments[Locations::MiseryMireMedallion] = medallions[mt_rand(0, 2)];
  assignments[Locations::TurtleRockMedallion] = medallions[mt_rand(0, 2)];
}

void fillPrizes() {
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
    assignments[prize_locations[i]] = prizes[i];
  }
}

void makeseed(int seed) {
  cout << seed << endl;
  cout << seed << endl;
  setMedallions();
  fillPrizes();
}

int main(int argc, char **argv) {
  int seed;
  cin >> seed;
  makeseed(seed);
}
