#include <string>

#ifndef _LOCATION_H
#define _LOCATION_H
enum class Location {
  INVALID,
  AginahsCave,
  Blacksmith,
  BlindsHideoutFarLeft,
  BlindsHideoutFarRight,
  BlindsHideoutLeft,
  BlindsHideoutRight,
  BlindsHideoutTop,
  BombosTablet,
  BottleMerchant,
  Brewery,
  BumperCave,
  CShapedHouse,
  CastleTowerDarkMaze,
  CastleTowerRoom03,
  Catfish,
  Cave45,
  CheckerboardCave,
  ChestGame,
  ChickenHouse,
  DarkWorldNorthEastPrize,
  DesertLedge,
  DesertPalaceBigChest,
  DesertPalaceBigKeyChest,
  DesertPalaceCompassChest,
  DesertPalaceLanmolas,
  DesertPalaceMapChest,
  DesertPalacePrize,
  DesertPalaceTorch,
  DiggingGame,
  EasternPalaceArmosKnights,
  EasternPalaceBigChest,
  EasternPalaceBigKeyChest,
  EasternPalaceCannonballChest,
  EasternPalaceCompassChest,
  EasternPalaceMapChest,
  EasternPalacePrize,
  EtherTablet,
  FloatingIsland,
  FloodgateChest,
  FluteSpot,
  GanonsTowerBigChest,
  GanonsTowerBigKeyChest,
  GanonsTowerBigKeyRoomLeft,
  GanonsTowerBigKeyRoomRight,
  GanonsTowerBobsChest,
  GanonsTowerBobsTorch,
  GanonsTowerCompassRoomBottomLeft,
  GanonsTowerCompassRoomBottomRight,
  GanonsTowerCompassRoomTopLeft,
  GanonsTowerCompassRoomTopRight,
  GanonsTowerDMsRoomBottomLeft,
  GanonsTowerDMsRoomBottomRight,
  GanonsTowerDMsRoomTopLeft,
  GanonsTowerDMsRoomTopRight,
  GanonsTowerFiresnakeRoom,
  GanonsTowerHopeRoomLeft,
  GanonsTowerHopeRoomRight,
  GanonsTowerMapChest,
  GanonsTowerMiniHelmasaurRoomLeft,
  GanonsTowerMiniHelmasaurRoomRight,
  GanonsTowerMoldormChest,
  GanonsTowerPreMoldormChest,
  GanonsTowerPrize,
  GanonsTowerRandomizerRoomBottomLeft,
  GanonsTowerRandomizerRoomBottomRight,
  GanonsTowerRandomizerRoomTopLeft,
  GanonsTowerRandomizerRoomTopRight,
  GanonsTowerTileRoom,
  GraveyardLedge,
  HammerPegs,
  Hobo,
  HookshotCaveBottomLeft,
  HookshotCaveBottomRight,
  HookshotCaveTopLeft,
  HookshotCaveTopRight,
  HypeCaveBottom,
  HypeCaveMiddleLeft,
  HypeCaveMiddleRight,
  HypeCaveNPC,
  HypeCaveTop,
  HyruleCastleBoomerangChest,
  HyruleCastleMapChest,
  HyruleCastleTowerPrize,
  HyruleCastleZeldasCell,
  IcePalaceBigChest,
  IcePalaceBigKeyChest,
  IcePalaceCompassChest,
  IcePalaceFreezorChest,
  IcePalaceIcedTRoom,
  IcePalaceKholdstare,
  IcePalaceMapChest,
  IcePalacePrize,
  IcePalaceSpikeRoom,
  IceRodCave,
  KakarikoTavern,
  KakrikoWellBottom,
  KakrikoWellLeft,
  KakrikoWellMiddle,
  KakrikoWellRight,
  KakrikoWellTop,
  KingZora,
  KingsTomb,
  LakeHyliaIsland,
  Library,
  LinksHouse,
  LinksUncle,
  LostWoodsHideout,
  LumberjackTree,
  MagicBat,
  MasterSwordPedestal,
  MazeRace,
  MimicCave,
  MiniMoldormCaveFarLeft,
  MiniMoldormCaveFarRight,
  MiniMoldormCaveLeft,
  MiniMoldormCaveNPC,
  MiniMoldormCaveRight,
  MireShedLeft,
  MireShedRight,
  MiseryMireBigChest,
  MiseryMireBigKeyChest,
  MiseryMireBridgeChest,
  MiseryMireCompassChest,
  MiseryMireMainLobby,
  MiseryMireMapChest,
  MiseryMireMedallion,
  MiseryMirePrize,
  MiseryMireSpikeChest,
  MiseryMireVitreous,
  Mushroom,
  OldMan,
  PalaceofDarknessBigChest,
  PalaceofDarknessBigKeyChest,
  PalaceofDarknessCompassChest,
  PalaceofDarknessDarkBasementLeft,
  PalaceofDarknessDarkBasementRight,
  PalaceofDarknessDarkMazeBottom,
  PalaceofDarknessDarkMazeTop,
  PalaceofDarknessHarmlessHellway,
  PalaceofDarknessHelmasaurKing,
  PalaceofDarknessMapChest,
  PalaceofDarknessPrize,
  PalaceofDarknessShooterRoom,
  PalaceofDarknessStalfosBasement,
  PalaceofDarknessTheArenaBridge,
  PalaceofDarknessTheArenaLedge,
  ParadoxCaveLowerFarLeft,
  ParadoxCaveLowerFarRight,
  ParadoxCaveLowerLeft,
  ParadoxCaveLowerMiddle,
  ParadoxCaveLowerRight,
  ParadoxCaveUpperLeft,
  ParadoxCaveUpperRight,
  PegasusRocks,
  PotionShop,
  PurpleChest,
  Pyramid,
  PyramidFairyBow,
  PyramidFairyLeft,
  PyramidFairyRight,
  PyramidFairySword,
  Sahasrahla,
  SahasrahlasHutLeft,
  SahasrahlasHutMiddle,
  SahasrahlasHutRight,
  Sanctuary,
  SecretPassage,
  SewersDarkCross,
  SewersSecretRoomLeft,
  SewersSecretRoomMiddle,
  SewersSecretRoomRight,
  SickKid,
  SkullWoodsBigChest,
  SkullWoodsBigKeyChest,
  SkullWoodsBridgeRoom,
  SkullWoodsCompassChest,
  SkullWoodsMapChest,
  SkullWoodsMothula,
  SkullWoodsPinballRoom,
  SkullWoodsPotPrison,
  SkullWoodsPrize,
  SpectacleRock,
  SpectacleRockCave,
  SpikeCave,
  SpiralCave,
  Stumpy,
  SunkenTreasure,
  SuperbunnyCaveBottom,
  SuperbunnyCaveTop,
  SwampPalaceArrghus,
  SwampPalaceBigChest,
  SwampPalaceBigKeyChest,
  SwampPalaceCompassChest,
  SwampPalaceEntrance,
  SwampPalaceFloodedRoomLeft,
  SwampPalaceFloodedRoomRight,
  SwampPalaceMapChest,
  SwampPalacePrize,
  SwampPalaceWaterfallRoom,
  SwampPalaceWestChest,
  ThievesTownAmbushChest,
  ThievesTownAttic,
  ThievesTownBigChest,
  ThievesTownBigKeyChest,
  ThievesTownBlind,
  ThievesTownBlindsCell,
  ThievesTownCompassChest,
  ThievesTownMapChest,
  ThievesTownPrize,
  TowerofHeraBasementCage,
  TowerofHeraBigChest,
  TowerofHeraBigKeyChest,
  TowerofHeraCompassChest,
  TowerofHeraMapChest,
  TowerofHeraMoldorm,
  TowerofHeraPrize,
  TurtleRockBigChest,
  TurtleRockBigKeyChest,
  TurtleRockChainChomps,
  TurtleRockCompassChest,
  TurtleRockCrystarollerRoom,
  TurtleRockEyeBridgeBottomLeft,
  TurtleRockEyeBridgeBottomRight,
  TurtleRockEyeBridgeTopLeft,
  TurtleRockEyeBridgeTopRight,
  TurtleRockMedallion,
  TurtleRockPrize,
  TurtleRockRollerRoomLeft,
  TurtleRockRollerRoomRight,
  TurtleRockTrinexx,
  WaterfallFairyLeft,
  WaterfallFairyRight,
  ZorasLedge,
  NUM_LOCATIONS,  // Must be last
};

enum class Region {
  INVALID,

  HyruleCastleEscape,
  EasternPalace,
  DesertPalace,
  TowerofHera,
  HyruleCastleTower,

  PalaceofDarkness,
  SwampPalace,
  SkullWoods,
  ThievesTown,
  IcePalace,
  MiseryMire,
  TurtleRock,
  GanonsTower,

  DarkWorldDeathMountainEast,
  DarkWorldDeathMountainWest,
  DarkWorldMire,
  DarkWorldNorthEast,
  DarkWorldNorthWest,
  DarkWorldSouth,
  DeathMountainEast,
  DeathMountainWest,
  LightWorld,
  Mire,
  NorthEastDarkWorld,
  NorthWestDarkWorld,
  SouthDarkWorld,
  WestDeathMountain,
  NUM_REGIONS,  // Must be last
};

const Location PRIZE_LOCATIONS[] = {
    Location::PalaceofDarknessPrize, Location::SwampPalacePrize,
    Location::SkullWoodsPrize,       Location::ThievesTownPrize,
    Location::IcePalacePrize,        Location::MiseryMirePrize,
    Location::TurtleRockPrize,       Location::EasternPalacePrize,
    Location::DesertPalacePrize,     Location::TowerofHeraPrize,
};

const int MAX_DUNGEON_LOCATIONS = 28;
const Location DUNGEON_LOCATIONS[NUM_DUNGEONS + 1][MAX_DUNGEON_LOCATIONS + 1] =
    {
        {
            // We access this with the Region enum, so make entry 0 a dummy.
            Location::INVALID,
        },
        {
            Location::SewersDarkCross,
            Location::SewersSecretRoomLeft,
            Location::SewersSecretRoomMiddle,
            Location::SewersSecretRoomRight,
            Location::HyruleCastleBoomerangChest,
            Location::HyruleCastleMapChest,
            Location::HyruleCastleZeldasCell,
            Location::INVALID,
        },
        {
            Location::EasternPalaceArmosKnights,
            Location::EasternPalaceBigChest,
            Location::EasternPalaceBigKeyChest,
            Location::EasternPalaceCannonballChest,
            Location::EasternPalaceCompassChest,
            Location::EasternPalaceMapChest,
            Location::EasternPalacePrize,
            Location::INVALID,
        },
        {
            Location::DesertPalaceBigChest,
            Location::DesertPalaceBigKeyChest,
            Location::DesertPalaceCompassChest,
            Location::DesertPalaceLanmolas,
            Location::DesertPalaceMapChest,
            Location::DesertPalacePrize,
            Location::DesertPalaceTorch,
            Location::INVALID,
        },
        {
            Location::TowerofHeraBasementCage,
            Location::TowerofHeraBigChest,
            Location::TowerofHeraBigKeyChest,
            Location::TowerofHeraCompassChest,
            Location::TowerofHeraMapChest,
            Location::TowerofHeraMoldorm,
            Location::TowerofHeraPrize,
            Location::INVALID,
        },
        {
            Location::CastleTowerDarkMaze,
            Location::CastleTowerRoom03,
            Location::HyruleCastleTowerPrize,
            Location::INVALID,
        },
        {
            Location::PalaceofDarknessBigChest,
            Location::PalaceofDarknessBigKeyChest,
            Location::PalaceofDarknessCompassChest,
            Location::PalaceofDarknessDarkBasementLeft,
            Location::PalaceofDarknessDarkBasementRight,
            Location::PalaceofDarknessDarkMazeBottom,
            Location::PalaceofDarknessDarkMazeTop,
            Location::PalaceofDarknessHarmlessHellway,
            Location::PalaceofDarknessHelmasaurKing,
            Location::PalaceofDarknessMapChest,
            Location::PalaceofDarknessPrize,
            Location::PalaceofDarknessShooterRoom,
            Location::PalaceofDarknessStalfosBasement,
            Location::PalaceofDarknessTheArenaBridge,
            Location::PalaceofDarknessTheArenaLedge,
            Location::INVALID,
        },
        {
            Location::SwampPalaceArrghus,
            Location::SwampPalaceBigChest,
            Location::SwampPalaceBigKeyChest,
            Location::SwampPalaceCompassChest,
            Location::SwampPalaceEntrance,
            Location::SwampPalaceFloodedRoomLeft,
            Location::SwampPalaceFloodedRoomRight,
            Location::SwampPalaceMapChest,
            Location::SwampPalacePrize,
            Location::SwampPalaceWaterfallRoom,
            Location::SwampPalaceWestChest,
            Location::INVALID,
        },
        {
            Location::SkullWoodsBigChest,
            Location::SkullWoodsBigKeyChest,
            Location::SkullWoodsBridgeRoom,
            Location::SkullWoodsCompassChest,
            Location::SkullWoodsMapChest,
            Location::SkullWoodsMothula,
            Location::SkullWoodsPinballRoom,
            Location::SkullWoodsPotPrison,
            Location::SkullWoodsPrize,
            Location::INVALID,
        },
        {
            Location::ThievesTownAmbushChest,
            Location::ThievesTownAttic,
            Location::ThievesTownBigChest,
            Location::ThievesTownBigKeyChest,
            Location::ThievesTownBlind,
            Location::ThievesTownBlindsCell,
            Location::ThievesTownCompassChest,
            Location::ThievesTownMapChest,
            Location::ThievesTownPrize,
            Location::INVALID,
        },
        {
            Location::IcePalaceBigChest,
            Location::IcePalaceBigKeyChest,
            Location::IcePalaceCompassChest,
            Location::IcePalaceFreezorChest,
            Location::IcePalaceIcedTRoom,
            Location::IcePalaceKholdstare,
            Location::IcePalaceMapChest,
            Location::IcePalacePrize,
            Location::IcePalaceSpikeRoom,
            Location::INVALID,
        },
        {
            Location::MiseryMireBigChest,
            Location::MiseryMireBigKeyChest,
            Location::MiseryMireBridgeChest,
            Location::MiseryMireCompassChest,
            Location::MiseryMireMainLobby,
            Location::MiseryMireMapChest,
            Location::MiseryMireMedallion,
            Location::MiseryMirePrize,
            Location::MiseryMireSpikeChest,
            Location::MiseryMireVitreous,
            Location::INVALID,
        },
        {
            Location::TurtleRockBigChest,
            Location::TurtleRockBigKeyChest,
            Location::TurtleRockChainChomps,
            Location::TurtleRockCompassChest,
            Location::TurtleRockCrystarollerRoom,
            Location::TurtleRockEyeBridgeBottomLeft,
            Location::TurtleRockEyeBridgeBottomRight,
            Location::TurtleRockEyeBridgeTopLeft,
            Location::TurtleRockEyeBridgeTopRight,
            Location::TurtleRockMedallion,
            Location::TurtleRockPrize,
            Location::TurtleRockRollerRoomLeft,
            Location::TurtleRockRollerRoomRight,
            Location::TurtleRockTrinexx,
            Location::INVALID,
        },
        {
            Location::GanonsTowerBigChest,
            Location::GanonsTowerBigKeyChest,
            Location::GanonsTowerBigKeyRoomLeft,
            Location::GanonsTowerBigKeyRoomRight,
            Location::GanonsTowerBobsChest,
            Location::GanonsTowerBobsTorch,
            Location::GanonsTowerCompassRoomBottomLeft,
            Location::GanonsTowerCompassRoomBottomRight,
            Location::GanonsTowerCompassRoomTopLeft,
            Location::GanonsTowerCompassRoomTopRight,
            Location::GanonsTowerDMsRoomBottomLeft,
            Location::GanonsTowerDMsRoomBottomRight,
            Location::GanonsTowerDMsRoomTopLeft,
            Location::GanonsTowerDMsRoomTopRight,
            Location::GanonsTowerFiresnakeRoom,
            Location::GanonsTowerHopeRoomLeft,
            Location::GanonsTowerHopeRoomRight,
            Location::GanonsTowerMapChest,
            Location::GanonsTowerMiniHelmasaurRoomLeft,
            Location::GanonsTowerMiniHelmasaurRoomRight,
            Location::GanonsTowerMoldormChest,
            Location::GanonsTowerPreMoldormChest,
            Location::GanonsTowerPrize,
            Location::GanonsTowerRandomizerRoomBottomLeft,
            Location::GanonsTowerRandomizerRoomBottomRight,
            Location::GanonsTowerRandomizerRoomTopLeft,
            Location::GanonsTowerRandomizerRoomTopRight,
            Location::GanonsTowerTileRoom,
            Location::INVALID,
        },
};

const std::string LOCATION_NAMES[] = {
    "INVALID",
    "AginahsCave",
    "Blacksmith",
    "BlindsHideoutFarLeft",
    "BlindsHideoutFarRight",
    "BlindsHideoutLeft",
    "BlindsHideoutRight",
    "BlindsHideoutTop",
    "BombosTablet",
    "BottleMerchant",
    "Brewery",
    "BumperCave",
    "CShapedHouse",
    "CastleTowerDarkMaze",
    "CastleTowerRoom03",
    "Catfish",
    "Cave45",
    "CheckerboardCave",
    "ChestGame",
    "ChickenHouse",
    "DarkWorldNorthEastPrize",
    "DesertLedge",
    "DesertPalaceBigChest",
    "DesertPalaceBigKeyChest",
    "DesertPalaceCompassChest",
    "DesertPalaceLanmolas",
    "DesertPalaceMapChest",
    "DesertPalacePrize",
    "DesertPalaceTorch",
    "DiggingGame",
    "EasternPalaceArmosKnights",
    "EasternPalaceBigChest",
    "EasternPalaceBigKeyChest",
    "EasternPalaceCannonballChest",
    "EasternPalaceCompassChest",
    "EasternPalaceMapChest",
    "EasternPalacePrize",
    "EtherTablet",
    "FloatingIsland",
    "FloodgateChest",
    "FluteSpot",
    "GanonsTowerBigChest",
    "GanonsTowerBigKeyChest",
    "GanonsTowerBigKeyRoomLeft",
    "GanonsTowerBigKeyRoomRight",
    "GanonsTowerBobsChest",
    "GanonsTowerBobsTorch",
    "GanonsTowerCompassRoomBottomLeft",
    "GanonsTowerCompassRoomBottomRight",
    "GanonsTowerCompassRoomTopLeft",
    "GanonsTowerCompassRoomTopRight",
    "GanonsTowerDMsRoomBottomLeft",
    "GanonsTowerDMsRoomBottomRight",
    "GanonsTowerDMsRoomTopLeft",
    "GanonsTowerDMsRoomTopRight",
    "GanonsTowerFiresnakeRoom",
    "GanonsTowerHopeRoomLeft",
    "GanonsTowerHopeRoomRight",
    "GanonsTowerMapChest",
    "GanonsTowerMiniHelmasaurRoomLeft",
    "GanonsTowerMiniHelmasaurRoomRight",
    "GanonsTowerMoldormChest",
    "GanonsTowerPreMoldormChest",
    "GanonsTowerPrize",
    "GanonsTowerRandomizerRoomBottomLeft",
    "GanonsTowerRandomizerRoomBottomRight",
    "GanonsTowerRandomizerRoomTopLeft",
    "GanonsTowerRandomizerRoomTopRight",
    "GanonsTowerTileRoom",
    "GraveyardLedge",
    "HammerPegs",
    "Hobo",
    "HookshotCaveBottomLeft",
    "HookshotCaveBottomRight",
    "HookshotCaveTopLeft",
    "HookshotCaveTopRight",
    "HypeCaveBottom",
    "HypeCaveMiddleLeft",
    "HypeCaveMiddleRight",
    "HypeCaveNPC",
    "HypeCaveTop",
    "HyruleCastleBoomerangChest",
    "HyruleCastleMapChest",
    "HyruleCastleTowerPrize",
    "HyruleCastleZeldasCell",
    "IcePalaceBigChest",
    "IcePalaceBigKeyChest",
    "IcePalaceCompassChest",
    "IcePalaceFreezorChest",
    "IcePalaceIcedTRoom",
    "IcePalaceKholdstare",
    "IcePalaceMapChest",
    "IcePalacePrize",
    "IcePalaceSpikeRoom",
    "IceRodCave",
    "KakarikoTavern",
    "KakrikoWellBottom",
    "KakrikoWellLeft",
    "KakrikoWellMiddle",
    "KakrikoWellRight",
    "KakrikoWellTop",
    "KingZora",
    "KingsTomb",
    "LakeHyliaIsland",
    "Library",
    "LinksHouse",
    "LinksUncle",
    "LostWoodsHideout",
    "LumberjackTree",
    "MagicBat",
    "MasterSwordPedestal",
    "MazeRace",
    "MimicCave",
    "MiniMoldormCaveFarLeft",
    "MiniMoldormCaveFarRight",
    "MiniMoldormCaveLeft",
    "MiniMoldormCaveNPC",
    "MiniMoldormCaveRight",
    "MireShedLeft",
    "MireShedRight",
    "MiseryMireBigChest",
    "MiseryMireBigKeyChest",
    "MiseryMireBridgeChest",
    "MiseryMireCompassChest",
    "MiseryMireMainLobby",
    "MiseryMireMapChest",
    "MiseryMireMedallion",
    "MiseryMirePrize",
    "MiseryMireSpikeChest",
    "MiseryMireVitreous",
    "Mushroom",
    "OldMan",
    "PalaceofDarknessBigChest",
    "PalaceofDarknessBigKeyChest",
    "PalaceofDarknessCompassChest",
    "PalaceofDarknessDarkBasementLeft",
    "PalaceofDarknessDarkBasementRight",
    "PalaceofDarknessDarkMazeBottom",
    "PalaceofDarknessDarkMazeTop",
    "PalaceofDarknessHarmlessHellway",
    "PalaceofDarknessHelmasaurKing",
    "PalaceofDarknessMapChest",
    "PalaceofDarknessPrize",
    "PalaceofDarknessShooterRoom",
    "PalaceofDarknessStalfosBasement",
    "PalaceofDarknessTheArenaBridge",
    "PalaceofDarknessTheArenaLedge",
    "ParadoxCaveLowerFarLeft",
    "ParadoxCaveLowerFarRight",
    "ParadoxCaveLowerLeft",
    "ParadoxCaveLowerMiddle",
    "ParadoxCaveLowerRight",
    "ParadoxCaveUpperLeft",
    "ParadoxCaveUpperRight",
    "PegasusRocks",
    "PotionShop",
    "PurpleChest",
    "Pyramid",
    "PyramidFairyBow",
    "PyramidFairyLeft",
    "PyramidFairyRight",
    "PyramidFairySword",
    "Sahasrahla",
    "SahasrahlasHutLeft",
    "SahasrahlasHutMiddle",
    "SahasrahlasHutRight",
    "Sanctuary",
    "SecretPassage",
    "SewersDarkCross",
    "SewersSecretRoomLeft",
    "SewersSecretRoomMiddle",
    "SewersSecretRoomRight",
    "SickKid",
    "SkullWoodsBigChest",
    "SkullWoodsBigKeyChest",
    "SkullWoodsBridgeRoom",
    "SkullWoodsCompassChest",
    "SkullWoodsMapChest",
    "SkullWoodsMothula",
    "SkullWoodsPinballRoom",
    "SkullWoodsPotPrison",
    "SkullWoodsPrize",
    "SpectacleRock",
    "SpectacleRockCave",
    "SpikeCave",
    "SpiralCave",
    "Stumpy",
    "SunkenTreasure",
    "SuperbunnyCaveBottom",
    "SuperbunnyCaveTop",
    "SwampPalaceArrghus",
    "SwampPalaceBigChest",
    "SwampPalaceBigKeyChest",
    "SwampPalaceCompassChest",
    "SwampPalaceEntrance",
    "SwampPalaceFloodedRoomLeft",
    "SwampPalaceFloodedRoomRight",
    "SwampPalaceMapChest",
    "SwampPalacePrize",
    "SwampPalaceWaterfallRoom",
    "SwampPalaceWestChest",
    "ThievesTownAmbushChest",
    "ThievesTownAttic",
    "ThievesTownBigChest",
    "ThievesTownBigKeyChest",
    "ThievesTownBlind",
    "ThievesTownBlindsCell",
    "ThievesTownCompassChest",
    "ThievesTownMapChest",
    "ThievesTownPrize",
    "TowerofHeraBasementCage",
    "TowerofHeraBigChest",
    "TowerofHeraBigKeyChest",
    "TowerofHeraCompassChest",
    "TowerofHeraMapChest",
    "TowerofHeraMoldorm",
    "TowerofHeraPrize",
    "TurtleRockBigChest",
    "TurtleRockBigKeyChest",
    "TurtleRockChainChomps",
    "TurtleRockCompassChest",
    "TurtleRockCrystarollerRoom",
    "TurtleRockEyeBridgeBottomLeft",
    "TurtleRockEyeBridgeBottomRight",
    "TurtleRockEyeBridgeTopLeft",
    "TurtleRockEyeBridgeTopRight",
    "TurtleRockMedallion",
    "TurtleRockPrize",
    "TurtleRockRollerRoomLeft",
    "TurtleRockRollerRoomRight",
    "TurtleRockTrinexx",
    "WaterfallFairyLeft",
    "WaterfallFairyRight",
    "ZorasLedge",
};

#endif
