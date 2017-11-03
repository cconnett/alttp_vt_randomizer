<?php namespace ALttP\Filler;

use ALttP\Filler;
use ALttP\Support\ItemCollection as Items;
use ALttP\ItemBag;
use Log;

class RandomAssumed extends Filler {
	/**
	 * This fill places items in the first available location that it can possibly be in, assuming that unplaced
	 * items will be reachable. Those items will then have a smaller set of places that they can be placed.
	 *
	 * @param array $dungeon items that must be placed
	 * @param array $required items that must be placed
	 * @param array $nice items that would be nice to have placed
	 * @param array $extra items that don't matter if they get placed
	 *
	 * @return null
	 */
	public function fill(array $dungeon, array $required, array $nice, array $extra) {
		$randomized_order_locations = $this->shuffleLocations($this->world->getEmptyLocations());

		$this->fillItemsInLocations($dungeon, $randomized_order_locations, $required);

		// random junk fill
		$gt_locations = $this->world->getRegion('Ganons Tower')->getEmptyLocations()->randomCollection(mt_rand(0, 15));
		$extra = $this->shuffleItems($extra);
		$trash = array_splice($extra, 0, $gt_locations->count());
		$this->fastFillItemsInLocations($trash, $gt_locations);

		$randomized_order_locations = $randomized_order_locations->getEmptyLocations()->reverse();

		$this->fillItemsInLocations($this->shuffleItems($required), $randomized_order_locations);

		$randomized_order_locations = $this->shuffleLocations($randomized_order_locations->getEmptyLocations());

		$this->fastFillItemsInLocations($this->shuffleItems($nice), $randomized_order_locations);

		$this->fastFillItemsInLocations($this->shuffleItems($extra), $randomized_order_locations->getEmptyLocations());
	}

	protected function fillItemsInLocations($fill_items, $locations_to_fill, $base_assumed_items = []) {
		$remaining_fill_items = new Items($fill_items);
		Log::debug(sprintf("Filling %s items in %s locations", $remaining_fill_items->count(),
                           $locations_to_fill->getEmptyLocations()->count()));

		if ($remaining_fill_items->count() > $locations_to_fill->getEmptyLocations()->count()) {
            throw new \Exception("Trying to fill more items than available locations.");
        }
        $remaining_fill_items = $remaining_fill_items->merge($base_assumed_items);

        $all_locations = $this->world->getLocations();
        $item_bag = new ItemBag($all_locations, $fill_items);

        // The items to fill are already in a shuffled order.
        foreach ($fill_items as $key => $item) {
          // Instead of collecting all the items, query the current
          // placements.

          // Step through canidate locations and place the item in the
          // first one reachable with the items we assumed in the
          // previous step.
          $fill_location = null;
          foreach($locations_to_fill as $location) {
            if ($location->hasItem()) {
              continue;
            }
            // Tentatively place it here by setting it in $item_placements;
            $item_bag->tentativelyFillLocation($location, $item);
            // $location->setItem($item);
            if ($location->canFill($item, $item_bag)) {
              $fill_location = $location;
              print($location->getName());
              $item_bag->lockTentative();
              break;
            } else {
              if ($item->getNiceName() == "Palace of Darkness Compass") {
                if ($location->getRegion()->getName() == "Dark Palace") {
                  $item_bag->dump();
                  print(call_user_func($location->requirement_callback, $locations_to_fill, $item_bag) . ">");
                }
              }
              $item_bag->undoTentative();
              // $location->setItem(null);
            }
          }
          if ($fill_location === null) {
            throw new \Exception(sprintf('No Available Locations: "%s"', $item->getNiceName()));
          }
          Log::debug(sprintf("Placing Item: %s in %s", $item->getNiceName(), $fill_location->getName()));
          $fill_location->setItem($item);
          print("===================\n");
		}
              }
}
