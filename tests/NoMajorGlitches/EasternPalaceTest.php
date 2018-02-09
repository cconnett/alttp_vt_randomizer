<?php namespace NoMajorGlitches;

use ALttP\Item;
use ALttP\World;
use TestCase;

/**
 * @group NMG
 */
class EasternPalaceTest extends TestCase {
	public function setUp() {
		parent::setUp();
		$this->world = new World('test_rules', 'NoMajorGlitches');
	}

	public function tearDown() {
		parent::tearDown();
		unset($this->world);
	}

	// Entry
	public function testNothingRequiredToEnter() {
		$this->assertTrue($this->world->getRegion('Eastern Palace')
			->canEnter($this->world->getLocations(), $this->collected));
	}

	/**
	 * @param string $location
	 * @param bool $access
	 * @param array $items
	 * @param array $except
	 *
	 * @dataProvider accessPool
	 */
	public function testLocation(string $location, bool $access, array $items, array $except = []) {
		if (count($except)) {
			$this->collected = $this->allItemsExcept($except);
		}

		$this->addCollected($items);

		$this->assertEquals($access, $this->world->getLocation($location)
			->canAccess($this->collected));
	}

	/**
	 * @param string $location
	 * @param bool $access
	 * @param string $item
	 * @param array $items
	 * @param array $except
	 *
	 * @dataProvider fillPool
	 */
	public function testFillLocation(string $location, bool $access, string $item, array $items = [], array $except = []) {
		if (count($except)) {
			$this->collected = $this->allItemsExcept($except);
		}

		$this->addCollected($items);

		$this->assertEquals($access, $this->world->getLocation($location)
			->fill(Item::get($item), $this->collected));
	}

	public function fillPool() {
		return [
			["Eastern Palace - Compass Chest", true, 'BigKeyP1', [], ['BigKeyP1']],

			["Eastern Palace - Cannonball Chest", true, 'BigKeyP1', [], ['BigKeyP1']],

			["Eastern Palace - Big Chest", false, 'BigKeyP1', [], ['BigKeyP1']],

			["Eastern Palace - Map Chest", true, 'BigKeyP1', [], ['BigKeyP1']],

			["Eastern Palace - Big Key Chest", true, 'BigKeyP1', [], ['BigKeyP1']],

			["Eastern Palace - Armos Knights", false, 'BigKeyP1', [], ['BigKeyP1']],
		];
	}

	public function accessPool() {
		return [
			["Eastern Palace - Compass Chest", true, []],

			["Eastern Palace - Cannonball Chest", true, []],

			["Eastern Palace - Big Chest", false, []],
			["Eastern Palace - Big Chest", false, [], ['BigKeyP1']],
			["Eastern Palace - Big Chest", true, ['BigKeyP1']],

			["Eastern Palace - Map Chest", true, []],

			["Eastern Palace - Big Key Chest", false, []],
			["Eastern Palace - Big Key Chest", false, [], ['Lamp']],
			["Eastern Palace - Big Key Chest", true, ['Lamp']],


			["Eastern Palace - Armos Knights", false, []],
			["Eastern Palace - Armos Knights", false, [], ['Lamp']],
			["Eastern Palace - Armos Knights", false, [], ['AnyBow']],
			["Eastern Palace - Armos Knights", false, [], ['BigKeyP1']],
			["Eastern Palace - Armos Knights", true, ['Lamp', 'Bow', 'BigKeyP1']],
		];
	}

	/**
	 * @dataProvider dungeonItemsPool
	 */
	public function testRegionLockedItems(bool $access, Item $item, bool $free = null, string $config = null) {
		if ($config) {
			config(["alttp.test_rules.$config" => $free]);
		}

		$this->assertEquals($access, $this->world->getRegion('Eastern Palace')->canFill($item));
	}

	public function dungeonItemsPool() {
		return [
			[true, Item::get('Key')],
			[false, Item::get('KeyH2')],
			[false, Item::get('KeyH1')],
			[true, Item::get('KeyP1')],
			[false, Item::get('KeyP2')],
			[false, Item::get('KeyA1')],
			[false, Item::get('KeyD2')],
			[false, Item::get('KeyD1')],
			[false, Item::get('KeyD6')],
			[false, Item::get('KeyD3')],
			[false, Item::get('KeyD5')],
			[false, Item::get('KeyP3')],
			[false, Item::get('KeyD4')],
			[false, Item::get('KeyD7')],
			[false, Item::get('KeyA2')],

			[true, Item::get('BigKey')],
			[false, Item::get('BigKeyH2')],
			[false, Item::get('BigKeyH1')],
			[true, Item::get('BigKeyP1')],
			[false, Item::get('BigKeyP2')],
			[false, Item::get('BigKeyA1')],
			[false, Item::get('BigKeyD2')],
			[false, Item::get('BigKeyD1')],
			[false, Item::get('BigKeyD6')],
			[false, Item::get('BigKeyD3')],
			[false, Item::get('BigKeyD5')],
			[false, Item::get('BigKeyP3')],
			[false, Item::get('BigKeyD4')],
			[false, Item::get('BigKeyD7')],
			[false, Item::get('BigKeyA2')],

			[true, Item::get('Map'), false, 'region.wildMaps'],
			[true, Item::get('Map'), true, 'region.wildMaps'],
			[false, Item::get('MapH2'), false, 'region.wildMaps'],
			[true, Item::get('MapH2'), true, 'region.wildMaps'],
			[false, Item::get('MapH1'), false, 'region.wildMaps'],
			[true, Item::get('MapH1'), true, 'region.wildMaps'],
			[true, Item::get('MapP1'), false, 'region.wildMaps'],
			[true, Item::get('MapP1'), true, 'region.wildMaps'],
			[false, Item::get('MapP2'), false, 'region.wildMaps'],
			[true, Item::get('MapP2'), true, 'region.wildMaps'],
			[false, Item::get('MapA1'), false, 'region.wildMaps'],
			[true, Item::get('MapA1'), true, 'region.wildMaps'],
			[false, Item::get('MapD2'), false, 'region.wildMaps'],
			[true, Item::get('MapD2'), true, 'region.wildMaps'],
			[false, Item::get('MapD1'), false, 'region.wildMaps'],
			[true, Item::get('MapD1'), true, 'region.wildMaps'],
			[false, Item::get('MapD6'), false, 'region.wildMaps'],
			[true, Item::get('MapD6'), true, 'region.wildMaps'],
			[false, Item::get('MapD3'), false, 'region.wildMaps'],
			[true, Item::get('MapD3'), true, 'region.wildMaps'],
			[false, Item::get('MapD5'), false, 'region.wildMaps'],
			[true, Item::get('MapD5'), true, 'region.wildMaps'],
			[false, Item::get('MapP3'), false, 'region.wildMaps'],
			[true, Item::get('MapP3'), true, 'region.wildMaps'],
			[false, Item::get('MapD4'), false, 'region.wildMaps'],
			[true, Item::get('MapD4'), true, 'region.wildMaps'],
			[false, Item::get('MapD7'), false, 'region.wildMaps'],
			[true, Item::get('MapD7'), true, 'region.wildMaps'],
			[false, Item::get('MapA2'), false, 'region.wildMaps'],
			[true, Item::get('MapA2'), true, 'region.wildMaps'],

			[true, Item::get('Compass'), false, 'region.wildCompasses'],
			[true, Item::get('Compass'), true, 'region.wildCompasses'],
			[false, Item::get('CompassH2'), false, 'region.wildCompasses'],
			[true, Item::get('CompassH2'), true, 'region.wildCompasses'],
			[false, Item::get('CompassH1'), false, 'region.wildCompasses'],
			[true, Item::get('CompassH1'), true, 'region.wildCompasses'],
			[true, Item::get('CompassP1'), false, 'region.wildCompasses'],
			[true, Item::get('CompassP1'), true, 'region.wildCompasses'],
			[false, Item::get('CompassP2'), false, 'region.wildCompasses'],
			[true, Item::get('CompassP2'), true, 'region.wildCompasses'],
			[false, Item::get('CompassA1'), false, 'region.wildCompasses'],
			[true, Item::get('CompassA1'), true, 'region.wildCompasses'],
			[false, Item::get('CompassD2'), false, 'region.wildCompasses'],
			[true, Item::get('CompassD2'), true, 'region.wildCompasses'],
			[false, Item::get('CompassD1'), false, 'region.wildCompasses'],
			[true, Item::get('CompassD1'), true, 'region.wildCompasses'],
			[false, Item::get('CompassD6'), false, 'region.wildCompasses'],
			[true, Item::get('CompassD6'), true, 'region.wildCompasses'],
			[false, Item::get('CompassD3'), false, 'region.wildCompasses'],
			[true, Item::get('CompassD3'), true, 'region.wildCompasses'],
			[false, Item::get('CompassD5'), false, 'region.wildCompasses'],
			[true, Item::get('CompassD5'), true, 'region.wildCompasses'],
			[false, Item::get('CompassP3'), false, 'region.wildCompasses'],
			[true, Item::get('CompassP3'), true, 'region.wildCompasses'],
			[false, Item::get('CompassD4'), false, 'region.wildCompasses'],
			[true, Item::get('CompassD4'), true, 'region.wildCompasses'],
			[false, Item::get('CompassD7'), false, 'region.wildCompasses'],
			[true, Item::get('CompassD7'), true, 'region.wildCompasses'],
			[false, Item::get('CompassA2'), false, 'region.wildCompasses'],
			[true, Item::get('CompassA2'), true, 'region.wildCompasses'],
		];
	}
}
