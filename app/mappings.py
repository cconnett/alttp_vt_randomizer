import itertools

items =[
  'INVALID',
  'Arrow',
  'ArrowUpgrade10',
  'ArrowUpgrade5',
  'ArrowUpgrade70',
  'BigKey',
  'BigKeyA1',
  'BigKeyA2',
  'BigKeyD1',
  'BigKeyD2',
  'BigKeyD3',
  'BigKeyD4',
  'BigKeyD5',
  'BigKeyD6',
  'BigKeyD7',
  'BigKeyH1',
  'BigKeyH2',
  'BigKeyP1',
  'BigKeyP2',
  'BigKeyP3',
  'BlueClock',
  'BlueMail',
  'BlueShield',
  'Bomb',
  'Bombos',
  'BombUpgrade10',
  'BombUpgrade5',
  'BombUpgrade50',
  'BookOfMudora',
  'Boomerang',
  'BossHeartContainer',
  'Bottle',
  'BottleWithBee',
  'BottleWithBluePotion',
  'BottleWithFairy',
  'BottleWithGoldBee',
  'BottleWithGreenPotion',
  'BottleWithRedPotion',
  'Bow',
  'BowAndArrows',
  'BowAndSilverArrows',
  'BugCatchingNet',
  'CaneOfByrna',
  'CaneOfSomaria',
  'Cape',
  'Compass',
  'CompassA2',
  'CompassD1',
  'CompassD2',
  'CompassD3',
  'CompassD4',
  'CompassD5',
  'CompassD6',
  'CompassD7',
  'CompassP1',
  'CompassP2',
  'CompassP3',
  'Crystal1',
  'Crystal2',
  'Crystal3',
  'Crystal4',
  'Crystal5',
  'Crystal6',
  'Crystal7',
  'DefeatAgahnim',
  'DefeatAgahnim2',
  'DefeatGanon',
  'Ether',
  'FiftyRupees',
  'FireRod',
  'FiveRupees',
  'Flippers',
  'GreenClock',
  'HalfMagic',
  'Hammer',
  'Heart',
  'HeartContainer',
  'HeartContainerNoAnimation',
  'Hookshot',
  'IceRod',
  'Key',
  'KeyA1',
  'KeyA2',
  'KeyD1',
  'KeyD2',
  'KeyD3',
  'KeyD4',
  'KeyD5',
  'KeyD6',
  'KeyD7',
  'KeyH1',
  'KeyH2',
  'KeyP1',
  'KeyP2',
  'KeyP3',
  'L1Sword',
  'L1SwordAndShield',
  'L2Sword',
  'L3Sword',
  'L4Sword',
  'Lamp',
  'MagicMirror',
  'MapA2',
  'MapD1',
  'MapD2',
  'MapD3',
  'MapD4',
  'MapD5',
  'MapD6',
  'MapD7',
  'MapH2',
  'MapP1',
  'MapP2',
  'MapP3',
  'MasterSword',
  'MirrorShield',
  'MoonPearl',
  'multiRNG',
  'Mushroom',
  'Nothing',
  'OcarinaActive',
  'OcarinaInactive',
  'OneHundredRupees',
  'OneRupee',
  'PegasusBoots',
  'PendantOfCourage',
  'PendantOfPower',
  'PendantOfWisdom',
  'PieceOfHeart',
  'Powder',
  'PowerGlove',
  'PowerStar',
  'ProgressiveArmor',
  'ProgressiveGlove',
  'ProgressiveShield',
  'ProgressiveSword',
  'Quake',
  'QuarterMagic',
  'RedBoomerang',
  'RedClock',
  'RedMail',
  'RedShield',
  'RescueZelda',
  'Rupoor',
  'Shovel',
  'SilverArrowUpgrade',
  'singleRNG',
  'SmallMagic',
  'TenArrows',
  'TenBombs',
  'ThreeBombs',
  'ThreeHundredRupees',
  'TitansMitt',
  'Triforce',
  'TriforcePiece',
  'TwentyRupees',
  'TwentyRupees2',
]

locations = [
  'INVALID',
  'AginahsCave',
  'Blacksmith',
  'BlindsHideoutFarLeft',
  'BlindsHideoutFarRight',
  'BlindsHideoutLeft',
  'BlindsHideoutRight',
  'BlindsHideoutTop',
  'BombosTablet',
  'BottleMerchant',
  'Brewery',
  'BumperCave',
  'CShapedHouse',
  'CastleTowerDarkMaze',
  'CastleTowerRoom03',
  'Catfish',
  'Cave45',
  'CheckerboardCave',
  'ChestGame',
  'ChickenHouse',
  'DarkWorldNorthEastPrize',
  'DesertLedge',
  'DesertPalaceBigChest',
  'DesertPalaceBigKeyChest',
  'DesertPalaceCompassChest',
  'DesertPalaceLanmolas',
  'DesertPalaceMapChest',
  'DesertPalacePrize',
  'DesertPalaceTorch',
  'DiggingGame',
  'EasternPalaceArmosKnights',
  'EasternPalaceBigChest',
  'EasternPalaceBigKeyChest',
  'EasternPalaceCannonballChest',
  'EasternPalaceCompassChest',
  'EasternPalaceMapChest',
  'EasternPalacePrize',
  'EtherTablet',
  'FloatingIsland',
  'FloodgateChest',
  'FluteSpot',
  'GanonsTowerBigChest',
  'GanonsTowerBigKeyChest',
  'GanonsTowerBigKeyRoomLeft',
  'GanonsTowerBigKeyRoomRight',
  'GanonsTowerBobsChest',
  'GanonsTowerBobsTorch',
  'GanonsTowerCompassRoomBottomLeft',
  'GanonsTowerCompassRoomBottomRight',
  'GanonsTowerCompassRoomTopLeft',
  'GanonsTowerCompassRoomTopRight',
  'GanonsTowerDMsRoomBottomLeft',
  'GanonsTowerDMsRoomBottomRight',
  'GanonsTowerDMsRoomTopLeft',
  'GanonsTowerDMsRoomTopRight',
  'GanonsTowerFiresnakeRoom',
  'GanonsTowerHopeRoomLeft',
  'GanonsTowerHopeRoomRight',
  'GanonsTowerMapChest',
  'GanonsTowerMiniHelmasaurRoomLeft',
  'GanonsTowerMiniHelmasaurRoomRight',
  'GanonsTowerMoldormChest',
  'GanonsTowerPreMoldormChest',
  'GanonsTowerPrize',
  'GanonsTowerRandomizerRoomBottomLeft',
  'GanonsTowerRandomizerRoomBottomRight',
  'GanonsTowerRandomizerRoomTopLeft',
  'GanonsTowerRandomizerRoomTopRight',
  'GanonsTowerTileRoom',
  'GraveyardLedge',
  'HammerPegs',
  'Hobo',
  'HookshotCaveBottomLeft',
  'HookshotCaveBottomRight',
  'HookshotCaveTopLeft',
  'HookshotCaveTopRight',
  'HypeCaveBottom',
  'HypeCaveMiddleLeft',
  'HypeCaveMiddleRight',
  'HypeCaveNPC',
  'HypeCaveTop',
  'HyruleCastleBoomerangChest',
  'HyruleCastleEscapePrize',
  'HyruleCastleMapChest',
  'HyruleCastleTowerPrize',
  'HyruleCastleZeldasCell',
  'IcePalaceBigChest',
  'IcePalaceBigKeyChest',
  'IcePalaceCompassChest',
  'IcePalaceFreezorChest',
  'IcePalaceIcedTRoom',
  'IcePalaceKholdstare',
  'IcePalaceMapChest',
  'IcePalacePrize',
  'IcePalaceSpikeRoom',
  'IceRodCave',
  'KakarikoTavern',
  'KakarikoWellBottom',
  'KakarikoWellLeft',
  'KakarikoWellMiddle',
  'KakarikoWellRight',
  'KakarikoWellTop',
  'KingZora',
  'KingsTomb',
  'LakeHyliaIsland',
  'Library',
  'LinksHouse',
  'LinksUncle',
  'LostWoodsHideout',
  'LumberjackTree',
  'MagicBat',
  'MasterSwordPedestal',
  'MazeRace',
  'MimicCave',
  'MiniMoldormCaveFarLeft',
  'MiniMoldormCaveFarRight',
  'MiniMoldormCaveLeft',
  'MiniMoldormCaveNPC',
  'MiniMoldormCaveRight',
  'MireShedLeft',
  'MireShedRight',
  'MiseryMireBigChest',
  'MiseryMireBigKeyChest',
  'MiseryMireBridgeChest',
  'MiseryMireCompassChest',
  'MiseryMireMainLobby',
  'MiseryMireMapChest',
  'MiseryMireMedallion',
  'MiseryMirePrize',
  'MiseryMireSpikeChest',
  'MiseryMireVitreous',
  'Mushroom',
  'OldMan',
  'PalaceofDarknessBigChest',
  'PalaceofDarknessBigKeyChest',
  'PalaceofDarknessCompassChest',
  'PalaceofDarknessDarkBasementLeft',
  'PalaceofDarknessDarkBasementRight',
  'PalaceofDarknessDarkMazeBottom',
  'PalaceofDarknessDarkMazeTop',
  'PalaceofDarknessHarmlessHellway',
  'PalaceofDarknessHelmasaurKing',
  'PalaceofDarknessMapChest',
  'PalaceofDarknessPrize',
  'PalaceofDarknessShooterRoom',
  'PalaceofDarknessStalfosBasement',
  'PalaceofDarknessTheArenaBridge',
  'PalaceofDarknessTheArenaLedge',
  'ParadoxCaveLowerFarLeft',
  'ParadoxCaveLowerFarRight',
  'ParadoxCaveLowerLeft',
  'ParadoxCaveLowerMiddle',
  'ParadoxCaveLowerRight',
  'ParadoxCaveUpperLeft',
  'ParadoxCaveUpperRight',
  'PegasusRocks',
  'PotionShop',
  'PurpleChest',
  'Pyramid',
  'PyramidFairyBow',
  'PyramidFairyLeft',
  'PyramidFairyRight',
  'PyramidFairySword',
  'Sahasrahla',
  'SahasrahlasHutLeft',
  'SahasrahlasHutMiddle',
  'SahasrahlasHutRight',
  'Sanctuary',
  'SecretPassage',
  'SewersDarkCross',
  'SewersSecretRoomLeft',
  'SewersSecretRoomMiddle',
  'SewersSecretRoomRight',
  'SickKid',
  'SkullWoodsBigChest',
  'SkullWoodsBigKeyChest',
  'SkullWoodsBridgeRoom',
  'SkullWoodsCompassChest',
  'SkullWoodsMapChest',
  'SkullWoodsMothula',
  'SkullWoodsPinballRoom',
  'SkullWoodsPotPrison',
  'SkullWoodsPrize',
  'SpectacleRock',
  'SpectacleRockCave',
  'SpikeCave',
  'SpiralCave',
  'Stumpy',
  'SunkenTreasure',
  'SuperbunnyCaveBottom',
  'SuperbunnyCaveTop',
  'SwampPalaceArrghus',
  'SwampPalaceBigChest',
  'SwampPalaceBigKeyChest',
  'SwampPalaceCompassChest',
  'SwampPalaceEntrance',
  'SwampPalaceFloodedRoomLeft',
  'SwampPalaceFloodedRoomRight',
  'SwampPalaceMapChest',
  'SwampPalacePrize',
  'SwampPalaceWaterfallRoom',
  'SwampPalaceWestChest',
  'ThievesTownAmbushChest',
  'ThievesTownAttic',
  'ThievesTownBigChest',
  'ThievesTownBigKeyChest',
  'ThievesTownBlind',
  'ThievesTownBlindsCell',
  'ThievesTownCompassChest',
  'ThievesTownMapChest',
  'ThievesTownPrize',
  'TowerofHeraBasementCage',
  'TowerofHeraBigChest',
  'TowerofHeraBigKeyChest',
  'TowerofHeraCompassChest',
  'TowerofHeraMapChest',
  'TowerofHeraMoldorm',
  'TowerofHeraPrize',
  'TurtleRockBigChest',
  'TurtleRockBigKeyChest',
  'TurtleRockChainChomps',
  'TurtleRockCompassChest',
  'TurtleRockCrystarollerRoom',
  'TurtleRockEyeBridgeBottomLeft',
  'TurtleRockEyeBridgeBottomRight',
  'TurtleRockEyeBridgeTopLeft',
  'TurtleRockEyeBridgeTopRight',
  'TurtleRockMedallion',
  'TurtleRockPrize',
  'TurtleRockRollerRoomLeft',
  'TurtleRockRollerRoomRight',
  'TurtleRockTrinexx',
  'WaterfallFairyLeft',
  'WaterfallFairyRight',
  'ZorasLedge',
]
counter = itertools.count()
items = dict(zip(items, counter))
items.update({value: name for (name, value) in items.items()})

counter = itertools.count()
locations = dict(zip(locations, counter))
locations.update({value: name for (name, value) in locations.items()})
locations['GroveCave'] = locations['Cave45']
