<?php namespace ALttP\Filler;

use ALttP\Filler;
use ALttP\Item;
use ALttP\Support\ItemCollection as Items;
use Log;

class RandomAssumed extends Filler {
	private $ganon_junk_lower = 0;
	private $ganon_junk_upper = 15;

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

		$this->fillItemsInLocations($dungeon, $randomized_order_locations, array_merge($required, $nice));

		// random junk fill
		$gt_locations = $this->world->getRegion('Ganons Tower')->getEmptyLocations()
			->randomCollection(mt_rand2($this->ganon_junk_lower, $this->ganon_junk_upper));
		$extra = $this->shuffleItems($extra);
		$trash = array_splice($extra, 0, $gt_locations->count());
		$this->fastFillItemsInLocations($trash, $gt_locations);

		$randomized_order_locations = $randomized_order_locations->getEmptyLocations()->reverse();

		$this->fillItemsInLocations($this->shuffleItems($required), $randomized_order_locations);

		$randomized_order_locations = $this->shuffleLocations($randomized_order_locations->getEmptyLocations());

		$this->fastFillItemsInLocations($this->shuffleItems($nice), $randomized_order_locations);

		$this->fastFillItemsInLocations($this->shuffleItems($extra), $randomized_order_locations->getEmptyLocations());
	}

	/**
	 * This fill places items in the first available location that it can possibly be in, assuming that unplaced
	 * items will be reachable. Those items will then have a smaller set of places that they can be placed.
	 *
	 * @param int $min minimum junk items to be placed
	 * @param int $max maximum junk items to be placed
	 *
	 * @return $this
	 */
	public function setGanonJunkLimits(int $min, int $max) {
		$this->ganon_junk_lower = $min;
		$this->ganon_junk_upper = $max;

		return $this;
	}

	protected function fillItemsInLocations($fill_items, $locations, $base_assumed_items = []) {
		$remaining_fill_items = new Items($fill_items, $this->world);
		Log::debug(sprintf("Filling %s items in %s locations", $remaining_fill_items->count(),
			$locations->getEmptyLocations()->count()));

		$this->world->setCurrentlyFillingItems($remaining_fill_items);

		if ($remaining_fill_items->count() > $locations->getEmptyLocations()->count()) {
			throw new \Exception("Trying to fill more items than available locations.");
		}

        // The items to fill are already in a shuffled order.
        foreach ($fill_items as $key => $item) {
            // Assume that Link has already collected:
            // 1. All unplaced items.
            // 2. All placed items that are iteratively reachable starting with
            //    $base_assumed_items.
            //
            // The one we're currently placing is not part of that.
            $assumed_items =
                $this->world->collectItems(
                    $remaining_fill_items->removeItem($item->getName())
                    ->merge($base_assumed_items));
            // Step through canidate locations and place the item in the
            // first one reachable with the items we assumed in the
            // previous step.

            // printf("Filling %s\n", $item->getName());
            // printf("Collected: %s\n", json_encode($assumed_items->map(
            // function($i){return $i->getName();}))) ;
			if ($item instanceof Item\Compass || $item instanceof Item\Map) {
              $fillable_locations = $locations->filter(function($location) use ($item, $assumed_items) {
				return !$location->hasItem() && $location->canFill($item, $assumed_items);
			  });
              $fill_location = $fillable_locations->random();
            } else {
              $fill_location = null;
              foreach($locations as $location) {
                if (!$location->hasItem() &&
                    $location->canFill($item, $assumed_items)) {
                  $fill_location = $location;
                  break;
                }
              }
			}

            if ($fill_location === null) {
              throw new \Exception(
                  sprintf('No Available Locations: "%s" %s',
                          $item->getNiceName(),
                          json_encode($remaining_fill_items->map(
                              function($i){return $i->getName();}))));
            }

			Log::debug(sprintf("Placing Item: %s in %s", $item->getNiceName(), $fill_location->getName()));
            $lname = preg_replace("/[^A-Za-z0-9]/", "", $fill_location->getName());
            $iname = preg_replace("/[^A-Za-z0-9]/", "", $item->getName());
            printf("%s := %s\n", $lname, $iname);
			$fill_location->setItem($item, true);
		}
	}
}
