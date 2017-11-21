#include <cstring>
#include <vector>

#include "items.h"
#include "locations.h"
#include "world.h"

using namespace std;

World::World() { memset(reachability_cache, 0, sizeof reachability_cache); }

void World::set_item(int location, int item) {
  this->assignments[location] = item;
  this->where_is[item].push_back(location);
  memset(this->reachability_cache, 0, sizeof this->reachability_cache);
}

bool World::can_reach(int location) {
  if (this->reachability_cache[location]) {
    return this->reachability_cache > 0;
  }
  // Set it to -1 while we calculate reachability so that loops in the graph
  // see it as unreachable.
  this->reachability_cache[location] = -1;

  switch (location) {
    // Generated code goes here.
    default:
      return true;
  }
}

int World::num_reachable(int item) {
  int count = 0;
  for (auto i = where_is[item].cbegin(); i != where_is[item].cend(); i++) {
    if (this->can_reach(*i)) {
      count += 1;
    }
  }
  return count;
}

bool World::can_enter(int region) {
  switch (region) {
    default:
      return false;
  }
}
