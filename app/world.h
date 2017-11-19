#include <vector>

using namespace std;

class World {
 public:
  World();

  void set_item(int location, int item);
  bool can_reach(int location);
  int num_reachable(int item);

 private:
  int assignments[Locations::NUM_LOCATIONS];
  vector<int> where_is[Items::NUM_ITEMS];

  // 1 = Reachable
  // 0 = Unknown
  // -1 = Unreachable
  int reachability_cache[Locations::NUM_LOCATIONS];
};
