;; IMPORTANT: MiseryMireMedallion and TurtleRockMedallion should be omitted.

(define-fun population_<n>
  ((i Item))
  Bool
  (and ((_ at-most <n>)
        (assigned AginahsCave i)
        (assigned Blacksmith i)
        (assigned BlindsHideoutFarLeft i)
        (assigned BlindsHideoutFarRight i)
        (assigned BlindsHideoutLeft i)
        (assigned BlindsHideoutRight i)
        (assigned BlindsHideoutTop i)
        (assigned BombosTablet i)
        (assigned BottleMerchant i)
        (assigned Brewery i)
        (assigned BumperCave i)
        (assigned CShapedHouse i)
        (assigned CastleTowerDarkMaze i)
        (assigned CastleTowerRoom03 i)
        (assigned Catfish i)
        (assigned Cave45 i)
        (assigned CheckerboardCave i)
        (assigned ChestGame i)
        (assigned ChickenHouse i)
        (assigned DesertLedge i)
        (assigned DesertPalaceBigChest i)
        (assigned DesertPalaceBigKeyChest i)
        (assigned DesertPalaceCompassChest i)
        (assigned DesertPalaceLanmolas i)
        (assigned DesertPalaceMapChest i)
        (assigned DesertPalacePrize i)
        (assigned DesertPalaceTorch i)
        (assigned DiggingGame i)
        (assigned EasternPalaceArmosKnights i)
        (assigned EasternPalaceBigChest i)
        (assigned EasternPalaceBigKeyChest i)
        (assigned EasternPalaceCannonballChest i)
        (assigned EasternPalaceCompassChest i)
        (assigned EasternPalaceMapChest i)
        (assigned EasternPalacePrize i)
        (assigned EtherTablet i)
        (assigned FloatingIsland i)
        (assigned FloodgateChest i)
        (assigned FluteSpot i)
        (assigned GanonsTowerBigChest i)
        (assigned GanonsTowerBigKeyChest i)
        (assigned GanonsTowerBigKeyRoomLeft i)
        (assigned GanonsTowerBigKeyRoomRight i)
        (assigned GanonsTowerBobsChest i)
        (assigned GanonsTowerBobsTorch i)
        (assigned GanonsTowerCompassRoomBottomLeft
                  i)
        (assigned GanonsTowerCompassRoomBottomRight
                  i)
        (assigned GanonsTowerCompassRoomTopLeft i)
        (assigned GanonsTowerCompassRoomTopRight i)
        (assigned GanonsTowerDMsRoomBottomLeft i)
        (assigned GanonsTowerDMsRoomBottomRight i)
        (assigned GanonsTowerDMsRoomTopLeft i)
        (assigned GanonsTowerDMsRoomTopRight i)
        (assigned GanonsTowerFiresnakeRoom i)
        (assigned GanonsTowerHopeRoomLeft i)
        (assigned GanonsTowerHopeRoomRight i)
        (assigned GanonsTowerMapChest i)
        (assigned GanonsTowerMiniHelmasaurRoomLeft
                  i)
        (assigned GanonsTowerMiniHelmasaurRoomRight
                  i)
        (assigned GanonsTowerMoldormChest i)
        (assigned GanonsTowerPreMoldormChest i)
        (assigned GanonsTowerRandomizerRoomBottomLeft
                  i)
        (assigned GanonsTowerRandomizerRoomBottomRight
                  i)
        (assigned GanonsTowerRandomizerRoomTopLeft
                  i)
        (assigned GanonsTowerRandomizerRoomTopRight
                  i)
        (assigned GanonsTowerTileRoom i)
        (assigned GraveyardLedge i)
        (assigned HammerPegs i)
        (assigned Hobo i)
        (assigned HookshotCaveBottomLeft i)
        (assigned HookshotCaveBottomRight i)
        (assigned HookshotCaveTopLeft i)
        (assigned HookshotCaveTopRight i)
        (assigned HypeCaveBottom i)
        (assigned HypeCaveMiddleLeft i)
        (assigned HypeCaveMiddleRight i)
        (assigned HypeCaveNPC i)
        (assigned HypeCaveTop i)
        (assigned HyruleCastleBoomerangChest i)
        (assigned HyruleCastleMapChest i)
        (assigned HyruleCastleZeldasCell i)
        (assigned IcePalaceBigChest i)
        (assigned IcePalaceBigKeyChest i)
        (assigned IcePalaceCompassChest i)
        (assigned IcePalaceFreezorChest i)
        (assigned IcePalaceIcedTRoom i)
        (assigned IcePalaceKholdstare i)
        (assigned IcePalaceMapChest i)
        (assigned IcePalacePrize i)
        (assigned IcePalaceSpikeRoom i)
        (assigned IceRodCave i)
        (assigned KakarikoTavern i)
        (assigned KakarikoWellBottom i)
        (assigned KakarikoWellLeft i)
        (assigned KakarikoWellMiddle i)
        (assigned KakarikoWellRight i)
        (assigned KakarikoWellTop i)
        (assigned KingZora i)
        (assigned KingsTomb i)
        (assigned LakeHyliaIsland i)
        (assigned Library i)
        (assigned LinksHouse i)
        (assigned LinksUncle i)
        (assigned LostWoodsHideout i)
        (assigned LumberjackTree i)
        (assigned MagicBat i)
        (assigned MasterSwordPedestal i)
        (assigned MazeRace i)
        (assigned MimicCave i)
        (assigned MiniMoldormCaveFarLeft i)
        (assigned MiniMoldormCaveFarRight i)
        (assigned MiniMoldormCaveLeft i)
        (assigned MiniMoldormCaveNPC i)
        (assigned MiniMoldormCaveRight i)
        (assigned MireShedLeft i)
        (assigned MireShedRight i)
        (assigned MiseryMireBigChest i)
        (assigned MiseryMireBigKeyChest i)
        (assigned MiseryMireBridgeChest i)
        (assigned MiseryMireCompassChest i)
        (assigned MiseryMireMainLobby i)
        (assigned MiseryMireMapChest i)
        (assigned MiseryMirePrize i)
        (assigned MiseryMireSpikeChest i)
        (assigned MiseryMireVitreous i)
        (assigned (as Mushroom Location)
                  i)
        (assigned OldMan i)
        (assigned PalaceofDarknessBigChest i)
        (assigned PalaceofDarknessBigKeyChest i)
        (assigned PalaceofDarknessCompassChest i)
        (assigned PalaceofDarknessDarkBasementLeft
                  i)
        (assigned PalaceofDarknessDarkBasementRight
                  i)
        (assigned PalaceofDarknessDarkMazeBottom i)
        (assigned PalaceofDarknessDarkMazeTop i)
        (assigned PalaceofDarknessHarmlessHellway
                  i)
        (assigned PalaceofDarknessHelmasaurKing i)
        (assigned PalaceofDarknessMapChest i)
        (assigned PalaceofDarknessPrize i)
        (assigned PalaceofDarknessShooterRoom i)
        (assigned PalaceofDarknessStalfosBasement
                  i)
        (assigned PalaceofDarknessTheArenaBridge i)
        (assigned PalaceofDarknessTheArenaLedge i)
        (assigned ParadoxCaveLowerFarLeft i)
        (assigned ParadoxCaveLowerFarRight i)
        (assigned ParadoxCaveLowerLeft i)
        (assigned ParadoxCaveLowerMiddle i)
        (assigned ParadoxCaveLowerRight i)
        (assigned ParadoxCaveUpperLeft i)
        (assigned ParadoxCaveUpperRight i)
        (assigned PegasusRocks i)
        (assigned PotionShop i)
        (assigned PurpleChest i)
        (assigned Pyramid i)
        (assigned PyramidFairyLeft i)
        (assigned PyramidFairyRight i)
        (assigned Sahasrahla i)
        (assigned SahasrahlasHutLeft i)
        (assigned SahasrahlasHutMiddle i)
        (assigned SahasrahlasHutRight i)
        (assigned Sanctuary i)
        (assigned SecretPassage i)
        (assigned SewersDarkCross i)
        (assigned SewersSecretRoomLeft i)
        (assigned SewersSecretRoomMiddle i)
        (assigned SewersSecretRoomRight i)
        (assigned SickKid i)
        (assigned SkullWoodsBigChest i)
        (assigned SkullWoodsBigKeyChest i)
        (assigned SkullWoodsBridgeRoom i)
        (assigned SkullWoodsCompassChest i)
        (assigned SkullWoodsMapChest i)
        (assigned SkullWoodsMothula i)
        (assigned SkullWoodsPinballRoom i)
        (assigned SkullWoodsPotPrison i)
        (assigned SkullWoodsPrize i)
        (assigned SpectacleRock i)
        (assigned SpectacleRockCave i)
        (assigned SpikeCave i)
        (assigned SpiralCave i)
        (assigned Stumpy i)
        (assigned SunkenTreasure i)
        (assigned SuperbunnyCaveBottom i)
        (assigned SuperbunnyCaveTop i)
        (assigned SwampPalaceArrghus i)
        (assigned SwampPalaceBigChest i)
        (assigned SwampPalaceBigKeyChest i)
        (assigned SwampPalaceCompassChest i)
        (assigned SwampPalaceEntrance i)
        (assigned SwampPalaceFloodedRoomLeft i)
        (assigned SwampPalaceFloodedRoomRight i)
        (assigned SwampPalaceMapChest i)
        (assigned SwampPalacePrize i)
        (assigned SwampPalaceWaterfallRoom i)
        (assigned SwampPalaceWestChest i)
        (assigned ThievesTownAmbushChest i)
        (assigned ThievesTownAttic i)
        (assigned ThievesTownBigChest i)
        (assigned ThievesTownBigKeyChest i)
        (assigned ThievesTownBlind i)
        (assigned ThievesTownBlindsCell i)
        (assigned ThievesTownCompassChest i)
        (assigned ThievesTownMapChest i)
        (assigned ThievesTownPrize i)
        (assigned TowerofHeraBasementCage i)
        (assigned TowerofHeraBigChest i)
        (assigned TowerofHeraBigKeyChest i)
        (assigned TowerofHeraCompassChest i)
        (assigned TowerofHeraMapChest i)
        (assigned TowerofHeraMoldorm i)
        (assigned TowerofHeraPrize i)
        (assigned TurtleRockBigChest i)
        (assigned TurtleRockBigKeyChest i)
        (assigned TurtleRockChainChomps i)
        (assigned TurtleRockCompassChest i)
        (assigned TurtleRockCrystarollerRoom i)
        (assigned TurtleRockEyeBridgeBottomLeft i)
        (assigned TurtleRockEyeBridgeBottomRight i)
        (assigned TurtleRockEyeBridgeTopLeft i)
        (assigned TurtleRockEyeBridgeTopRight i)
        (assigned TurtleRockPrize i)
        (assigned TurtleRockRollerRoomLeft i)
        (assigned TurtleRockRollerRoomRight i)
        (assigned TurtleRockTrinexx i)
        (assigned WaterfallFairyLeft i)
        (assigned WaterfallFairyRight i)
        (assigned ZorasLedge i))
       ((_ at-least <n>)
        (assigned AginahsCave i)
        (assigned Blacksmith i)
        (assigned BlindsHideoutFarLeft i)
        (assigned BlindsHideoutFarRight i)
        (assigned BlindsHideoutLeft i)
        (assigned BlindsHideoutRight i)
        (assigned BlindsHideoutTop i)
        (assigned BombosTablet i)
        (assigned BottleMerchant i)
        (assigned Brewery i)
        (assigned BumperCave i)
        (assigned CShapedHouse i)
        (assigned CastleTowerDarkMaze i)
        (assigned CastleTowerRoom03 i)
        (assigned Catfish i)
        (assigned Cave45 i)
        (assigned CheckerboardCave i)
        (assigned ChestGame i)
        (assigned ChickenHouse i)
        (assigned DesertLedge i)
        (assigned DesertPalaceBigChest i)
        (assigned DesertPalaceBigKeyChest i)
        (assigned DesertPalaceCompassChest i)
        (assigned DesertPalaceLanmolas i)
        (assigned DesertPalaceMapChest i)
        (assigned DesertPalacePrize i)
        (assigned DesertPalaceTorch i)
        (assigned DiggingGame i)
        (assigned EasternPalaceArmosKnights i)
        (assigned EasternPalaceBigChest i)
        (assigned EasternPalaceBigKeyChest i)
        (assigned EasternPalaceCannonballChest i)
        (assigned EasternPalaceCompassChest i)
        (assigned EasternPalaceMapChest i)
        (assigned EasternPalacePrize i)
        (assigned EtherTablet i)
        (assigned FloatingIsland i)
        (assigned FloodgateChest i)
        (assigned FluteSpot i)
        (assigned GanonsTowerBigChest i)
        (assigned GanonsTowerBigKeyChest i)
        (assigned GanonsTowerBigKeyRoomLeft i)
        (assigned GanonsTowerBigKeyRoomRight i)
        (assigned GanonsTowerBobsChest i)
        (assigned GanonsTowerBobsTorch i)
        (assigned GanonsTowerCompassRoomBottomLeft
                  i)
        (assigned GanonsTowerCompassRoomBottomRight
                  i)
        (assigned GanonsTowerCompassRoomTopLeft i)
        (assigned GanonsTowerCompassRoomTopRight i)
        (assigned GanonsTowerDMsRoomBottomLeft i)
        (assigned GanonsTowerDMsRoomBottomRight i)
        (assigned GanonsTowerDMsRoomTopLeft i)
        (assigned GanonsTowerDMsRoomTopRight i)
        (assigned GanonsTowerFiresnakeRoom i)
        (assigned GanonsTowerHopeRoomLeft i)
        (assigned GanonsTowerHopeRoomRight i)
        (assigned GanonsTowerMapChest i)
        (assigned GanonsTowerMiniHelmasaurRoomLeft
                  i)
        (assigned GanonsTowerMiniHelmasaurRoomRight
                  i)
        (assigned GanonsTowerMoldormChest i)
        (assigned GanonsTowerPreMoldormChest i)
        (assigned GanonsTowerRandomizerRoomBottomLeft
                  i)
        (assigned GanonsTowerRandomizerRoomBottomRight
                  i)
        (assigned GanonsTowerRandomizerRoomTopLeft
                  i)
        (assigned GanonsTowerRandomizerRoomTopRight
                  i)
        (assigned GanonsTowerTileRoom i)
        (assigned GraveyardLedge i)
        (assigned HammerPegs i)
        (assigned Hobo i)
        (assigned HookshotCaveBottomLeft i)
        (assigned HookshotCaveBottomRight i)
        (assigned HookshotCaveTopLeft i)
        (assigned HookshotCaveTopRight i)
        (assigned HypeCaveBottom i)
        (assigned HypeCaveMiddleLeft i)
        (assigned HypeCaveMiddleRight i)
        (assigned HypeCaveNPC i)
        (assigned HypeCaveTop i)
        (assigned HyruleCastleBoomerangChest i)
        (assigned HyruleCastleMapChest i)
        (assigned HyruleCastleZeldasCell i)
        (assigned IcePalaceBigChest i)
        (assigned IcePalaceBigKeyChest i)
        (assigned IcePalaceCompassChest i)
        (assigned IcePalaceFreezorChest i)
        (assigned IcePalaceIcedTRoom i)
        (assigned IcePalaceKholdstare i)
        (assigned IcePalaceMapChest i)
        (assigned IcePalacePrize i)
        (assigned IcePalaceSpikeRoom i)
        (assigned IceRodCave i)
        (assigned KakarikoTavern i)
        (assigned KakarikoWellBottom i)
        (assigned KakarikoWellLeft i)
        (assigned KakarikoWellMiddle i)
        (assigned KakarikoWellRight i)
        (assigned KakarikoWellTop i)
        (assigned KingZora i)
        (assigned KingsTomb i)
        (assigned LakeHyliaIsland i)
        (assigned Library i)
        (assigned LinksHouse i)
        (assigned LinksUncle i)
        (assigned LostWoodsHideout i)
        (assigned LumberjackTree i)
        (assigned MagicBat i)
        (assigned MasterSwordPedestal i)
        (assigned MazeRace i)
        (assigned MimicCave i)
        (assigned MiniMoldormCaveFarLeft i)
        (assigned MiniMoldormCaveFarRight i)
        (assigned MiniMoldormCaveLeft i)
        (assigned MiniMoldormCaveNPC i)
        (assigned MiniMoldormCaveRight i)
        (assigned MireShedLeft i)
        (assigned MireShedRight i)
        (assigned MiseryMireBigChest i)
        (assigned MiseryMireBigKeyChest i)
        (assigned MiseryMireBridgeChest i)
        (assigned MiseryMireCompassChest i)
        (assigned MiseryMireMainLobby i)
        (assigned MiseryMireMapChest i)
        (assigned MiseryMirePrize i)
        (assigned MiseryMireSpikeChest i)
        (assigned MiseryMireVitreous i)
        (assigned (as Mushroom Location)
                  i)
        (assigned OldMan i)
        (assigned PalaceofDarknessBigChest i)
        (assigned PalaceofDarknessBigKeyChest i)
        (assigned PalaceofDarknessCompassChest i)
        (assigned PalaceofDarknessDarkBasementLeft
                  i)
        (assigned PalaceofDarknessDarkBasementRight
                  i)
        (assigned PalaceofDarknessDarkMazeBottom i)
        (assigned PalaceofDarknessDarkMazeTop i)
        (assigned PalaceofDarknessHarmlessHellway
                  i)
        (assigned PalaceofDarknessHelmasaurKing i)
        (assigned PalaceofDarknessMapChest i)
        (assigned PalaceofDarknessPrize i)
        (assigned PalaceofDarknessShooterRoom i)
        (assigned PalaceofDarknessStalfosBasement
                  i)
        (assigned PalaceofDarknessTheArenaBridge i)
        (assigned PalaceofDarknessTheArenaLedge i)
        (assigned ParadoxCaveLowerFarLeft i)
        (assigned ParadoxCaveLowerFarRight i)
        (assigned ParadoxCaveLowerLeft i)
        (assigned ParadoxCaveLowerMiddle i)
        (assigned ParadoxCaveLowerRight i)
        (assigned ParadoxCaveUpperLeft i)
        (assigned ParadoxCaveUpperRight i)
        (assigned PegasusRocks i)
        (assigned PotionShop i)
        (assigned PurpleChest i)
        (assigned Pyramid i)
        (assigned PyramidFairyLeft i)
        (assigned PyramidFairyRight i)
        (assigned Sahasrahla i)
        (assigned SahasrahlasHutLeft i)
        (assigned SahasrahlasHutMiddle i)
        (assigned SahasrahlasHutRight i)
        (assigned Sanctuary i)
        (assigned SecretPassage i)
        (assigned SewersDarkCross i)
        (assigned SewersSecretRoomLeft i)
        (assigned SewersSecretRoomMiddle i)
        (assigned SewersSecretRoomRight i)
        (assigned SickKid i)
        (assigned SkullWoodsBigChest i)
        (assigned SkullWoodsBigKeyChest i)
        (assigned SkullWoodsBridgeRoom i)
        (assigned SkullWoodsCompassChest i)
        (assigned SkullWoodsMapChest i)
        (assigned SkullWoodsMothula i)
        (assigned SkullWoodsPinballRoom i)
        (assigned SkullWoodsPotPrison i)
        (assigned SkullWoodsPrize i)
        (assigned SpectacleRock i)
        (assigned SpectacleRockCave i)
        (assigned SpikeCave i)
        (assigned SpiralCave i)
        (assigned Stumpy i)
        (assigned SunkenTreasure i)
        (assigned SuperbunnyCaveBottom i)
        (assigned SuperbunnyCaveTop i)
        (assigned SwampPalaceArrghus i)
        (assigned SwampPalaceBigChest i)
        (assigned SwampPalaceBigKeyChest i)
        (assigned SwampPalaceCompassChest i)
        (assigned SwampPalaceEntrance i)
        (assigned SwampPalaceFloodedRoomLeft i)
        (assigned SwampPalaceFloodedRoomRight i)
        (assigned SwampPalaceMapChest i)
        (assigned SwampPalacePrize i)
        (assigned SwampPalaceWaterfallRoom i)
        (assigned SwampPalaceWestChest i)
        (assigned ThievesTownAmbushChest i)
        (assigned ThievesTownAttic i)
        (assigned ThievesTownBigChest i)
        (assigned ThievesTownBigKeyChest i)
        (assigned ThievesTownBlind i)
        (assigned ThievesTownBlindsCell i)
        (assigned ThievesTownCompassChest i)
        (assigned ThievesTownMapChest i)
        (assigned ThievesTownPrize i)
        (assigned TowerofHeraBasementCage i)
        (assigned TowerofHeraBigChest i)
        (assigned TowerofHeraBigKeyChest i)
        (assigned TowerofHeraCompassChest i)
        (assigned TowerofHeraMapChest i)
        (assigned TowerofHeraMoldorm i)
        (assigned TowerofHeraPrize i)
        (assigned TurtleRockBigChest i)
        (assigned TurtleRockBigKeyChest i)
        (assigned TurtleRockChainChomps i)
        (assigned TurtleRockCompassChest i)
        (assigned TurtleRockCrystarollerRoom i)
        (assigned TurtleRockEyeBridgeBottomLeft i)
        (assigned TurtleRockEyeBridgeBottomRight i)
        (assigned TurtleRockEyeBridgeTopLeft i)
        (assigned TurtleRockEyeBridgeTopRight i)
        (assigned TurtleRockPrize i)
        (assigned TurtleRockRollerRoomLeft i)
        (assigned TurtleRockRollerRoomRight i)
        (assigned TurtleRockTrinexx i)
        (assigned WaterfallFairyLeft i)
        (assigned WaterfallFairyRight i)
        (assigned ZorasLedge i))))
