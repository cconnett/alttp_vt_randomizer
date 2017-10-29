<?php namespace ALttP\Filler;

use ALttP\Filler;
use ALttP\Support\ItemCollection as Items;
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

	protected function fillItemsInLocations($fill_items, $locations, $base_assumed_items = []) {
		$remaining_fill_items = new Items($fill_items);
		Log::debug(sprintf("Filling %s items in %s locations", $remaining_fill_items->count(),
                           $locations->getEmptyLocations()->count()));

		if ($remaining_fill_items->count() > $locations->getEmptyLocations()->count()) {
            throw new \Exception("Trying to fill more items than available locations.");
        }
        $remaining_fill_items = $remaining_fill_items->merge($base_assumed_items);

        $filled_locations = $this->world->getCollectableLocations()->filter(function($location) {
			return $location->hasItem();
		});
        // The items to fill are already in a shuffled order.
		foreach ($fill_items as $key => $item) {
            // Assume that Link has already collected:
            // 1. All unplaced items.
            // 2. All placed items that are iteratively reachable starting with
            //    $base_assumed_items.
            //
            // The one we're currently placing is not part of that.
            $assumed_items = $this->world->collectItems(
                $remaining_fill_items->removeItem($item->getName()),
                $filled_locations);
            // Step through canidate locations and place the item in the
            // first one reachable with the items we assumed in the
            // previous step.
            $fill_location = null;
            foreach($locations as $location) {
                if (!$location->hasItem() &&
                    $location->canFill($item, $assumed_items)) {
                    $fill_location = $location;
                    break;
                }
			}
			if ($fill_location === null) {
				throw new \Exception(sprintf('No Available Locations: "%s"', $item->getNiceName()));
			}
			Log::debug(sprintf("Placing Item: %s in %s", $item->getNiceName(), $fill_location->getName()));
			$fill_location->setItem($item);
            $filled_locations->addItem($fill_location);

            throw new \Exception(sprintf('No Available Locations: "%s"', $item->getNiceName()));
		}
	}
}
