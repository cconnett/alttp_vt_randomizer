#include <vector>

#include "items.h"
#include "mt_rand.h"
#include "spdlog/spdlog.h"

using namespace std;

class World {
 public:
  World(int seed);
  ~World();

  void print();
  void compact_print();
  const Item *view_assignments() const;

  bool has_item(Location location);

  // Can `location` be reached?
  bool can_reach(Location location);

  // How many instances of `item` can be reached?
  int num_reachable(Item item);
  // Can `n` instances of `item` can be reached?
  int is_num_reachable(int n, Item item);

  // How many bottles can be reached (including variations with contents)?
  int bottle_count();

  // Can the specified region be entered.
  bool can_enter(Region region);

  // Can `region` be completed (its prize collected)?
  bool can_complete(Region region);

  // Can `item` legally exist in `location`?
  //
  // This represents only the conditions set by setFillRules on the location;
  // this method does not consider whether `item` is region-specific (e.g., a
  // region-specific item would be item KeyD1 in region PalaceOfDarkness).
  bool can_fill(Location location, Item item);

  // Override other reachability and placement checks to always allow
  bool always_allow(Location location, Item item);

 private:
  Item assignments[(int)Location::NUM_LOCATIONS];
  vector<Location> where_is[(int)Item::NUM_ITEMS];
  int num_unplaced[(int)Item::NUM_ITEMS];

  mt_rand *generator;

  void raw_set_item(Location location, Item item);
  // Assign to `location` the item `item`. Invalidate the reachability cache.
  void set_item(Location location, Item item);
  // Assign `item` only if there are unplaced instances available.
  bool check_and_set_item(Location location, Item item);

  // Manage the list of items to assume are reachable.
  void clear_assumed();
  void add_assumed(const Item *items, size_t n_items);
  void decr_assumed(Item item) {
    clear_reachability_cache();
    num_unplaced[(int)item]--;
  }
  void incr_assumed(Item item) {
    clear_reachability_cache();
    num_unplaced[(int)item]++;
  }

  // Assign to `location` the medallion `item`. Don't add it to the where_is
  // structure to avoid it being seen as collectible.
  void set_medallion(Location location, Item item);

  // 1 = Reachable
  // 0 = Unknown
  // -1 = Unreachable
  int reachability_cache[(int)Location::NUM_LOCATIONS];
  void clear_reachability_cache();

  // Uncached version that's easier to generate.
  bool uncached_can_reach(Location location);
  void fill_items_in_locations(const Item *items, Location *locations);
  void fast_fill_items_in_locations(const Item *items, size_t n,
                                    Location *locations);
  Item get_bottle(int filled);
  void set_medallions();
  void fill_prizes();

  // std::shared_ptr<spdlog::logger> log;
};
