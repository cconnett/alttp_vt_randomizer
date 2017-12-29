#ifndef _LOCATION_H
#define _LOCATION_H

#include <string>

#include "arraylength.h"

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

  // Dungeon item locality iterates over dungeons in this order from
  // HyruleCastleEscape to GanonsTower. The dungeons must be a contiguous block
  // at the beginning of the Region enum.
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

  LightWorld,
  DeathMountainEast,
  DeathMountainWest,

  DarkWorldNorthEast,
  DarkWorldNorthWest,
  DarkWorldSouth,

  DarkWorldDeathMountainEast,
  DarkWorldDeathMountainWest,
  DarkWorldMire,

  NUM_REGIONS,  // Must be last
};

const Location PRIZE_LOCATIONS[] = {
    Location::PalaceofDarknessPrize, Location::SwampPalacePrize,
    Location::SkullWoodsPrize,       Location::ThievesTownPrize,
    Location::IcePalacePrize,        Location::MiseryMirePrize,
    Location::TurtleRockPrize,       Location::EasternPalacePrize,
    Location::DesertPalacePrize,     Location::TowerofHeraPrize,
};

// The base order that the PHP code has before the shuffle.
const Location FILLABLE_LOCATIONS[] = {
    Location::MasterSwordPedestal,
    Location::LinksUncle,
    Location::SecretPassage,
    Location::KingsTomb,
    Location::FloodgateChest,
    Location::LinksHouse,
    Location::KakarikoTavern,
    Location::ChickenHouse,
    Location::AginahsCave,
    Location::SahasrahlasHutLeft,
    Location::SahasrahlasHutMiddle,
    Location::SahasrahlasHutRight,
    Location::KakrikoWellTop,
    Location::KakrikoWellLeft,
    Location::KakrikoWellMiddle,
    Location::KakrikoWellRight,
    Location::KakrikoWellBottom,
    Location::BlindsHideoutTop,
    Location::BlindsHideoutLeft,
    Location::BlindsHideoutRight,
    Location::BlindsHideoutFarLeft,
    Location::BlindsHideoutFarRight,
    Location::PegasusRocks,
    Location::MiniMoldormCaveFarLeft,
    Location::MiniMoldormCaveLeft,
    Location::MiniMoldormCaveRight,
    Location::MiniMoldormCaveFarRight,
    Location::IceRodCave,
    Location::BottleMerchant,
    Location::Sahasrahla,
    Location::MagicBat,
    Location::SickKid,
    Location::Hobo,
    Location::BombosTablet,
    Location::KingZora,
    Location::LostWoodsHideout,
    Location::LumberjackTree,
    Location::Cave45,
    Location::GraveyardLedge,
    Location::CheckerboardCave,
    Location::MiniMoldormCaveNPC,
    Location::Library,
    Location::Mushroom,
    Location::PotionShop,
    Location::MazeRace,
    Location::DesertLedge,
    Location::LakeHyliaIsland,
    Location::SunkenTreasure,
    Location::ZorasLedge,
    Location::FluteSpot,
    Location::WaterfallFairyLeft,
    Location::WaterfallFairyRight,
    Location::Sanctuary,
    Location::SewersSecretRoomLeft,
    Location::SewersSecretRoomMiddle,
    Location::SewersSecretRoomRight,
    Location::SewersDarkCross,
    Location::HyruleCastleBoomerangChest,
    Location::HyruleCastleMapChest,
    Location::HyruleCastleZeldasCell,
    Location::EasternPalaceCompassChest,
    Location::EasternPalaceBigChest,
    Location::EasternPalaceCannonballChest,
    Location::EasternPalaceBigKeyChest,
    Location::EasternPalaceMapChest,
    Location::EasternPalaceArmosKnights,
    Location::DesertPalaceBigChest,
    Location::DesertPalaceMapChest,
    Location::DesertPalaceTorch,
    Location::DesertPalaceBigKeyChest,
    Location::DesertPalaceCompassChest,
    Location::DesertPalaceLanmolas,
    Location::OldMan,
    Location::SpectacleRockCave,
    Location::EtherTablet,
    Location::SpectacleRock,
    Location::SpiralCave,
    Location::MimicCave,
    Location::ParadoxCaveLowerFarLeft,
    Location::ParadoxCaveLowerLeft,
    Location::ParadoxCaveLowerRight,
    Location::ParadoxCaveLowerFarRight,
    Location::ParadoxCaveLowerMiddle,
    Location::ParadoxCaveUpperLeft,
    Location::ParadoxCaveUpperRight,
    Location::FloatingIsland,
    Location::TowerofHeraBigKeyChest,
    Location::TowerofHeraBasementCage,
    Location::TowerofHeraMapChest,
    Location::TowerofHeraCompassChest,
    Location::TowerofHeraBigChest,
    Location::TowerofHeraMoldorm,
    Location::CastleTowerRoom03,
    Location::CastleTowerDarkMaze,
    Location::SuperbunnyCaveTop,
    Location::SuperbunnyCaveBottom,
    Location::HookshotCaveTopRight,
    Location::HookshotCaveTopLeft,
    Location::HookshotCaveBottomLeft,
    Location::HookshotCaveBottomRight,
    Location::SpikeCave,
    Location::Catfish,
    Location::Pyramid,
    Location::PyramidFairyLeft,
    Location::PyramidFairyRight,
    Location::Brewery,
    Location::CShapedHouse,
    Location::ChestGame,
    Location::HammerPegs,
    Location::BumperCave,
    Location::Blacksmith,
    Location::PurpleChest,
    Location::HypeCaveTop,
    Location::HypeCaveMiddleRight,
    Location::HypeCaveMiddleLeft,
    Location::HypeCaveBottom,
    Location::Stumpy,
    Location::HypeCaveNPC,
    Location::DiggingGame,
    Location::MireShedLeft,
    Location::MireShedRight,
    Location::PalaceofDarknessShooterRoom,
    Location::PalaceofDarknessBigKeyChest,
    Location::PalaceofDarknessTheArenaLedge,
    Location::PalaceofDarknessTheArenaBridge,
    Location::PalaceofDarknessStalfosBasement,
    Location::PalaceofDarknessMapChest,
    Location::PalaceofDarknessBigChest,
    Location::PalaceofDarknessCompassChest,
    Location::PalaceofDarknessHarmlessHellway,
    Location::PalaceofDarknessDarkBasementLeft,
    Location::PalaceofDarknessDarkBasementRight,
    Location::PalaceofDarknessDarkMazeTop,
    Location::PalaceofDarknessDarkMazeBottom,
    Location::PalaceofDarknessHelmasaurKing,
    Location::SwampPalaceEntrance,
    Location::SwampPalaceBigChest,
    Location::SwampPalaceBigKeyChest,
    Location::SwampPalaceMapChest,
    Location::SwampPalaceWestChest,
    Location::SwampPalaceCompassChest,
    Location::SwampPalaceFloodedRoomLeft,
    Location::SwampPalaceFloodedRoomRight,
    Location::SwampPalaceWaterfallRoom,
    Location::SwampPalaceArrghus,
    Location::SkullWoodsBigChest,
    Location::SkullWoodsBigKeyChest,
    Location::SkullWoodsCompassChest,
    Location::SkullWoodsMapChest,
    Location::SkullWoodsBridgeRoom,
    Location::SkullWoodsPotPrison,
    Location::SkullWoodsMothula,
    Location::ThievesTownAttic,
    Location::ThievesTownBigKeyChest,
    Location::ThievesTownMapChest,
    Location::ThievesTownCompassChest,
    Location::ThievesTownAmbushChest,
    Location::ThievesTownBigChest,
    Location::ThievesTownBlindsCell,
    Location::ThievesTownBlind,
    Location::IcePalaceBigKeyChest,
    Location::IcePalaceCompassChest,
    Location::IcePalaceMapChest,
    Location::IcePalaceSpikeRoom,
    Location::IcePalaceFreezorChest,
    Location::IcePalaceIcedTRoom,
    Location::IcePalaceBigChest,
    Location::IcePalaceKholdstare,
    Location::MiseryMireBigChest,
    Location::MiseryMireMainLobby,
    Location::MiseryMireBigKeyChest,
    Location::MiseryMireCompassChest,
    Location::MiseryMireBridgeChest,
    Location::MiseryMireMapChest,
    Location::MiseryMireSpikeChest,
    Location::MiseryMireVitreous,
    Location::TurtleRockChainChomps,
    Location::TurtleRockCompassChest,
    Location::TurtleRockRollerRoomLeft,
    Location::TurtleRockRollerRoomRight,
    Location::TurtleRockBigChest,
    Location::TurtleRockBigKeyChest,
    Location::TurtleRockCrystarollerRoom,
    Location::TurtleRockEyeBridgeBottomLeft,
    Location::TurtleRockEyeBridgeBottomRight,
    Location::TurtleRockEyeBridgeTopLeft,
    Location::TurtleRockEyeBridgeTopRight,
    Location::TurtleRockTrinexx,
    Location::GanonsTowerBobsTorch,
    Location::GanonsTowerDMsRoomTopLeft,
    Location::GanonsTowerDMsRoomTopRight,
    Location::GanonsTowerDMsRoomBottomLeft,
    Location::GanonsTowerDMsRoomBottomRight,
    Location::GanonsTowerRandomizerRoomTopLeft,
    Location::GanonsTowerRandomizerRoomTopRight,
    Location::GanonsTowerRandomizerRoomBottomLeft,
    Location::GanonsTowerRandomizerRoomBottomRight,
    Location::GanonsTowerFiresnakeRoom,
    Location::GanonsTowerMapChest,
    Location::GanonsTowerBigChest,
    Location::GanonsTowerHopeRoomLeft,
    Location::GanonsTowerHopeRoomRight,
    Location::GanonsTowerBobsChest,
    Location::GanonsTowerTileRoom,
    Location::GanonsTowerCompassRoomTopLeft,
    Location::GanonsTowerCompassRoomTopRight,
    Location::GanonsTowerCompassRoomBottomLeft,
    Location::GanonsTowerCompassRoomBottomRight,
    Location::GanonsTowerBigKeyChest,
    Location::GanonsTowerBigKeyRoomLeft,
    Location::GanonsTowerBigKeyRoomRight,
    Location::GanonsTowerMiniHelmasaurRoomLeft,
    Location::GanonsTowerMiniHelmasaurRoomRight,
    Location::GanonsTowerPreMoldormChest,
    Location::GanonsTowerMoldormChest,
};
const int NUM_FILLABLE_LOCATIONS = ARRAY_LENGTH(FILLABLE_LOCATIONS);

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
            Location::Sanctuary,
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
            Location::GanonsTowerBobsTorch,
            Location::GanonsTowerDMsRoomTopLeft,
            Location::GanonsTowerDMsRoomTopRight,
            Location::GanonsTowerDMsRoomBottomLeft,
            Location::GanonsTowerDMsRoomBottomRight,
            Location::GanonsTowerRandomizerRoomTopLeft,
            Location::GanonsTowerRandomizerRoomTopRight,
            Location::GanonsTowerRandomizerRoomBottomLeft,
            Location::GanonsTowerRandomizerRoomBottomRight,
            Location::GanonsTowerFiresnakeRoom,
            Location::GanonsTowerMapChest,
            Location::GanonsTowerBigChest,
            Location::GanonsTowerHopeRoomLeft,
            Location::GanonsTowerHopeRoomRight,
            Location::GanonsTowerBobsChest,
            Location::GanonsTowerTileRoom,
            Location::GanonsTowerCompassRoomTopLeft,
            Location::GanonsTowerCompassRoomTopRight,
            Location::GanonsTowerCompassRoomBottomLeft,
            Location::GanonsTowerCompassRoomBottomRight,
            Location::GanonsTowerBigKeyChest,
            Location::GanonsTowerBigKeyRoomLeft,
            Location::GanonsTowerBigKeyRoomRight,
            Location::GanonsTowerMiniHelmasaurRoomLeft,
            Location::GanonsTowerMiniHelmasaurRoomRight,
            Location::GanonsTowerPreMoldormChest,
            Location::GanonsTowerMoldormChest,
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
