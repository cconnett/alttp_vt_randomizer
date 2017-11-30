#include <vector>

using namespace std;

class World {
 public:
  World();
  void print();

  bool has_item(Location location);

  // Assign to `location` the item `item`. Invalidate the reachability cache.
  void set_item(Location location, Item item);

  // Assign to `location` the medallion `item`. Don't add it to the where_is
  // structure to avoid it being seen as collectible.
  void set_medallion(Location location, Item item);

  // Can `location` be reached?
  bool can_reach(Location location);
  // Can `location` be reached if there is one fewer unplaced `item` in the
  // world (for testing graph validity if the item would be placed in location).
  bool can_reach_with_one_fewer_item(Location location, Item item);

  // How many instances of `item` can be reached?
  int num_reachable(Item item);

  // Can the specified region be entered.
  bool can_enter(Region region);

  // Can `region` be completed (its prize collected)?
  bool can_complete(Region region);

 private:
  Item assignments[(int)Location::NUM_LOCATIONS];
  vector<Location> where_is[(int)Item::NUM_ITEMS];
  int num_unplaced[(int)Item::NUM_ITEMS];

  // 1 = Reachable
  // 0 = Unknown
  // -1 = Unreachable
  int reachability_cache[(int)Location::NUM_LOCATIONS];

  // Uncached version that's easier to generate.
  bool uncached_can_reach(Location location);
};
