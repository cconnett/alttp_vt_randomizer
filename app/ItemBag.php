<?php namespace ALttP;

use ALttP\Support\ItemCollection;

const UNPLACED = "Unplaced";

// Factory for unique "unplaced" locations representing a location for an
// unplaced item. It has no address, data, or region; and it's always reachable,
// so its requirement callback is constant true.
function MakeUnplacedLocation() {
  return new Location(UNPLACED, 0x289B0, null, new Region(new World('placeholder', 'NoMajorGlitches')),
                      function($locations) { return true; });
}

// A facade that behaves like an ItemCollection in its implementation of `has`,
// but it implements `has` by traversing the item-location graph and caching the
// results.

class ItemBag extends ItemCollection {
  // A mapping from item names (strings) to a list of location objects where
  // copies of that item are placed.
  protected $item_placements;

  // A mapping from item names to booleans. It's used as a cache of
  // calculated item reachability.
  // Key: Location name.
  // Value: true - known reachable
  //        false - known unreachable
  //        absent - unknown reachability
  private $location_reachability;

  private $tentative_item = null;
  private $tentative_index = null;

  function __construct($locations, $items) {
    // Reachability cache starts empty.
    $this->location_reachability = [];

    // Store this so we can pass it to `canAccess` later.
    $this->locations = $locations;

    // Build the item-to-location index.
    $this->item_placements = [];

    // Make an entry for every item in the game.
    foreach ($items as $item) {
      $this->item_placements[$item->getName()] = [];
    }

    // Add an "unplaced" entry in the multiplicity requested.
    foreach ($items as $item) {
      $this->item_placements[$item->getName()][] = MakeUnplacedLocation();
    }
  }

  function undoTentative() {
    //print("Undo " . $this->tentative_item->getName() . " at " . $this->item_placements[$this->tentative_item->getName()][$this->tentative_index]->getName() . "\n");
    //if ($this->tentative_item && $this->tentative_index) {
      $this->item_placements[$this->tentative_item->getName()][$this->tentative_index] = MakeUnplacedLocation();
      $this->tentative_item = null;
      $this->tentative_index = null;
      //}
  }


  function lockTentative() {
    //    print("Lock " . $this->tentative_item->getName() . " at " . $this->item_placements[$this->tentative_item->getName()][$this->tentative_index] . "\n");
    $this->tentative_item = null;
    $this->tentative_index = null;
  }

  function tentativelyFillLocation($location, $item) {
    $this->location_reachability = [];

    // if ($location->hasItem()) {
    //   $old_item_name = $location->getItem()->getName();
    //   if (array_key_exists($old_item_name, $this->item_placements) &&
    //       array_search($location, $this->item_placements[$old_item_name])) {
    //     $this->item_placements[$old_item_name] = array_splice(
    //         $this->item_placements[$old_item_name],
    //         array_search($location, $this->item_placements[$old_item_name]),
    //         1);
    //   }
    // }
    // $location->setItem($item);
    $unplaced = null;
    for ($i = 0; $i < count($this->item_placements[$item->getName()]); $i++) {
      if ($this->item_placements[$item->getName()][$i]->getName() == UNPLACED) {
        $unplaced = $i;
        break;
      }
    }
    if ($unplaced !== null) {
      //print("Mark tentative " . $item->getName() . " at " . $location->getName() . "\n");
      $this->item_placements[$item->getName()][$unplaced] = $location;
      $this->tentative_item = $item;
      $this->tentative_index = $unplaced;
    } else {
      $this->dump();
      throw new \Exception("No " . $item->getName() . " left.");

    }
    // array_push($this->item_placements[$item->getName()], $location);
  }

  // Determine whether $item_name item can be reached in the requested quantity
  // (whether Link 'has' that many of the item).
  private $depth = 0;

  function has($item_name, $quantity = 1) {
    for($i = 0; $i < $this->depth; $i++) {
      print("> ");
    }
    print("Requesting ". $quantity." ". $item_name."\n");
    $this->depth += 1;

    $locations_with_item =
        $this->item_placements[$item_name] ?? [MakeUnplacedLocation()];

    $found_quantity = 0;
    foreach ($locations_with_item as $location) {
      $location_name = $location->getName();
      // Check for a known reachability result and use it if found.
      if (array_key_exists($location_name, $this->location_reachability)) {
        $found_quantity += (int)$this->location_reachability[$location_name];
        continue;
      }
      // Recursively determine reachability by passing this ItemCollection
      // facade as a bag of items to the location's `canAccess` function. Set
      // the current node's reachability to false during the search, since
      // looping back to this location means it is not reachable. Then store the
      // computed answer in the cache and return it.

      $this->location_reachability[$location_name] = Item::get($item_name) instanceof Item\Key;
      $answer = (bool)$location->canAccess($this, $this->locations);
      $this->location_reachability[$location_name] = $answer;
      $found_quantity += (int)$answer;
    }
    $this->depth -= 1;
    for($i = 0; $i < $this->depth; $i++) {
      print("> ");
    }
    print("Found ". $found_quantity." ". $item_name."\n");
    print ("result:".($found_quantity >= $quantity)."\n");
    return $found_quantity >= $quantity;
  }

  function dump() {
    foreach ($this->item_placements as $item_name => $placement_list) {
      print($item_name . " => [");
      foreach($placement_list as $loc) {
        print_r($loc->getName() . ", ");
      }
      print("]\n");
    }
  }
}
