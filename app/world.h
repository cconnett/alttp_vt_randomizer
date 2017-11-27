#include <vector>

using namespace std;

class World {
 public:
  World();

  // Assign to `location` the item `item`. Invalidate the reachability cache.
  void set_item(Location location, Item item);

  // Can `location` be reached?
  bool can_reach(Location location);

  // How many instances of `item` can be reached?
  int num_reachable(Item item);

  // Can the specified region be entered.
  bool can_enter(Region region);

  // Can `region` be completed (its prize collected)?
  bool can_complete(Region region);

 private:
  Item assignments[(int)Location::NUM_LOCATIONS];
  vector<Location> where_is[(int)Item::NUM_ITEMS];

  // 1 = Reachable
  // 0 = Unknown
  // -1 = Unreachable
  int reachability_cache[(int)Location::NUM_LOCATIONS];

  // Uncached version that's easier to generate.
  bool uncached_can_reach(Location location);
};
