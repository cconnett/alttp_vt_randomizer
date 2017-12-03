#include "items.h"
#include "locations.h"
#include "world.h"

#include <iostream>
using namespace std;

bool World::can_enter(Region region) {
  switch (region) {
      // <SUB:can_enter>
    default:
      return true;
  }
}

bool World::can_complete(Region region) {
  switch (region) {
    // <SUB:can_complete>
    default:
      return true;
  }
}

bool World::can_fill(Location location, Item item) {
  switch (location) {
    // <SUB:can_fill>
    default:
      return true;
  }
}

bool World::uncached_can_reach(Location location) {
  switch (location) {
    // <SUB:can_reach>
    default:
      return true;
  }
}
