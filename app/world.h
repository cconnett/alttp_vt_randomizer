#include <vector>

using namespace std;

class World {
 public:
  World();

  // Assign to `location` the item `item`. Invalidate the reachability cache.
  void set_item(int location, int item);

  // Can `location` be reached?
  bool can_reach(int location);

  // How many instances of `item` can be reached?
  int num_reachable(int item);

  // Can the specified region be entered.
  bool can_enter(int region);

 private:
  int assignments[Locations::NUM_LOCATIONS];
  vector<int> where_is[Items::NUM_ITEMS];

  // 1 = Reachable
  // 0 = Unknown
  // -1 = Unreachable
  int reachability_cache[Locations::NUM_LOCATIONS];
};
