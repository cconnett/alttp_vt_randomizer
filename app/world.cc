#include <cstring>
#include <vector>

#include "items.h"
#include "locations.h"
#include "world.h"

using namespace std;

World::World() { memset(reachability_cache, 0, sizeof reachability_cache); }

void World::set_item(Location location, Item item) {
  this->assignments[(int)location] = item;
  this->where_is[(int)item].push_back(location);
  memset(this->reachability_cache, 0, sizeof this->reachability_cache);
}

bool World::can_reach(Location location) {
  if (this->reachability_cache[(int)location]) {
    return this->reachability_cache[(int)location] > 0;
  }
  // Set it to -1 while we calculate reachability so that loops in the graph
  // see it as unreachable.
  this->reachability_cache[(int)location] = -1;
  this->reachability_cache[(int)location] = this->uncached_can_reach(location);
  return this->reachability_cache[(int)location];
}

int World::num_reachable(Item item) {
  int count = 0;
  for (auto i = where_is[(int)item].cbegin(); i != where_is[(int)item].cend();
       i++) {
    cout << "'Item " << (int)item << "' -> 'Location " << (int)*i << "';"
         << endl;
    if (this->can_reach(*i)) {
      count += 1;
    }
  }
  return count;
}
