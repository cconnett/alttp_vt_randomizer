#ifndef ITEMS_H
#define ITEMS_H
enum class Item {
  INVALID,
  Arrow,
  ArrowUpgrade10,
  ArrowUpgrade5,
  ArrowUpgrade70,
  BigKey,
  BigKeyA1,
  BigKeyA2,
  BigKeyD1,
  BigKeyD2,
  BigKeyD3,
  BigKeyD4,
  BigKeyD5,
  BigKeyD6,
  BigKeyD7,
  BigKeyH1,
  BigKeyH2,
  BigKeyP1,
  BigKeyP2,
  BigKeyP3,
  BlueClock,
  BlueMail,
  BlueShield,
  Bomb,
  Bombos,
  BombUpgrade10,
  BombUpgrade5,
  BombUpgrade50,
  BookOfMudora,
  Boomerang,
  BossHeartContainer,
  Bottle,
  BottleWithBee,
  BottleWithBluePotion,
  BottleWithFairy,
  BottleWithGoldBee,
  BottleWithGreenPotion,
  BottleWithRedPotion,
  Bow,
  BowAndArrows,
  BowAndSilverArrows,
  BugCatchingNet,
  CaneOfByrna,
  CaneOfSomaria,
  Cape,
  Compass,
  CompassA2,
  CompassD1,
  CompassD2,
  CompassD3,
  CompassD4,
  CompassD5,
  CompassD6,
  CompassD7,
  CompassP1,
  CompassP2,
  CompassP3,
  Crystal1,
  Crystal2,
  Crystal3,
  Crystal4,
  Crystal5,
  Crystal6,
  Crystal7,
  DefeatAgahnim,
  DefeatAgahnim2,
  DefeatGanon,
  Ether,
  FiftyRupees,
  FireRod,
  FiveRupees,
  Flippers,
  GreenClock,
  HalfMagic,
  Hammer,
  Heart,
  HeartContainer,
  HeartContainerNoAnimation,
  Hookshot,
  IceRod,
  Key,
  KeyA1,
  KeyA2,
  KeyD1,
  KeyD2,
  KeyD3,
  KeyD4,
  KeyD5,
  KeyD6,
  KeyD7,
  KeyH1,
  KeyH2,
  KeyP1,
  KeyP2,
  KeyP3,
  L1Sword,
  L1SwordAndShield,
  L2Sword,
  L3Sword,
  L4Sword,
  Lamp,
  MagicMirror,
  MapA2,
  MapD1,
  MapD2,
  MapD3,
  MapD4,
  MapD5,
  MapD6,
  MapD7,
  MapH2,
  MapP1,
  MapP2,
  MapP3,
  MasterSword,
  MirrorShield,
  MoonPearl,
  multiRNG,
  Mushroom,
  Nothing,
  OcarinaActive,
  OcarinaInactive,
  OneHundredRupees,
  OneRupee,
  PegasusBoots,
  PendantOfCourage,
  PendantOfPower,
  PendantOfWisdom,
  PieceOfHeart,
  Powder,
  PowerGlove,
  PowerStar,
  ProgressiveArmor,
  ProgressiveGlove,
  ProgressiveShield,
  ProgressiveSword,
  Quake,
  QuarterMagic,
  RedBoomerang,
  RedClock,
  RedMail,
  RedShield,
  Rupoor,
  Shovel,
  SilverArrowUpgrade,
  singleRNG,
  SmallMagic,
  TenArrows,
  TenBombs,
  ThreeBombs,
  ThreeHundredRupees,
  TitansMitt,
  Triforce,
  TriforcePiece,
  TwentyRupees,
  TwentyRupees2,
  NUM_ITEMS,  // Must be last
};

const Item PRIZES[] = {
    Item::Crystal1,        Item::Crystal2,         Item::Crystal3,
    Item::Crystal4,        Item::Crystal5,         Item::Crystal6,
    Item::Crystal7,        Item::PendantOfCourage, Item::PendantOfPower,
    Item::PendantOfWisdom,
};

// There are four major pools that the filler uses: dungeon, advancement,
// nice-to-have, and trash. Each of these will have a static array definition
// because we're only caring about open mode.

const Item DUNGEON_ITEMS[] = {
    Item::BigKeyA2,  Item::BigKeyD1,  Item::BigKeyD2,  Item::BigKeyD3,
    Item::BigKeyD4,  Item::BigKeyD5,  Item::BigKeyD6,  Item::BigKeyD7,
    Item::BigKeyP1,  Item::BigKeyP2,  Item::BigKeyP3,

    Item::KeyA2,     Item::KeyA2,     Item::KeyA2,     Item::KeyA2,
    Item::KeyD1,     Item::KeyD1,     Item::KeyD1,     Item::KeyD1,
    Item::KeyD1,     Item::KeyD1,     Item::KeyD2,     Item::KeyD3,
    Item::KeyD3,     Item::KeyD4,     Item::KeyD5,     Item::KeyD5,
    Item::KeyD6,     Item::KeyD6,     Item::KeyD6,     Item::KeyD7,
    Item::KeyD7,     Item::KeyD7,     Item::KeyD7,     Item::KeyA1,
    Item::KeyA1,     Item::KeyH2,     Item::KeyP2,     Item::KeyP3,

    Item::MapA2,     Item::MapD1,     Item::MapD2,     Item::MapD3,
    Item::MapD4,     Item::MapD5,     Item::MapD6,     Item::MapD7,
    Item::MapH2,     Item::MapP1,     Item::MapP2,     Item::MapP3,

    Item::CompassA2, Item::CompassD1, Item::CompassD2, Item::CompassD3,
    Item::CompassD4, Item::CompassD5, Item::CompassD6, Item::CompassD7,
    Item::CompassP1, Item::CompassP2, Item::CompassP3,
};

// Advancement items should contain 2 progressive swords. The other two swords
// go in the nice-to-have category. SilverArrowUpgrade is in nice items.

const Item ADVANCEMENT_ITEMS[] = {
    Item::Bombos,
    Item::BookOfMudora,
    Item::Bottle,
    Item::Bow,
    Item::BugCatchingNet,
    Item::CaneOfByrna,
    Item::CaneOfSomaria,
    Item::Cape,
    Item::Ether,
    Item::FireRod,
    Item::Flippers,
    Item::HalfMagic,
    Item::Hammer,
    Item::Hookshot,
    Item::IceRod,
    Item::Lamp,
    Item::MagicMirror,
    Item::MoonPearl,
    Item::Mushroom,
    Item::OcarinaInactive,
    Item::PegasusBoots,
    Item::Powder,
    Item::ProgressiveGlove,
    Item::ProgressiveGlove,
    Item::ProgressiveShield,
    Item::ProgressiveShield,
    Item::ProgressiveShield,
    Item::ProgressiveSword,
    Item::ProgressiveSword,
    Item::Quake,
    Item::Shovel,
    Item::SilverArrowUpgrade,
};

const Item NICE_ITEMS[] = {
    Item::Bottle,
    Item::Bottle,
    Item::Bottle,

    Item::ProgressiveSword,
    Item::ProgressiveSword,

    Item::HeartContainer,
    Item::BossHeartContainer,
    Item::BossHeartContainer,
    Item::BossHeartContainer,
    Item::BossHeartContainer,
    Item::BossHeartContainer,
    Item::BossHeartContainer,
    Item::BossHeartContainer,
    Item::BossHeartContainer,
    Item::BossHeartContainer,
    Item::BossHeartContainer,

    Item::ProgressiveArmor,
    Item::ProgressiveArmor,

    Item::Boomerang,
    Item::RedBoomerang,
};

const Item TRASH_ITEMS[] = {
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,
    Item::PieceOfHeart,

    Item::BombUpgrade5,
    Item::BombUpgrade5,
    Item::BombUpgrade5,
    Item::BombUpgrade5,
    Item::BombUpgrade5,
    Item::BombUpgrade5,

    Item::BombUpgrade10,

    Item::ArrowUpgrade5,
    Item::ArrowUpgrade5,
    Item::ArrowUpgrade5,
    Item::ArrowUpgrade5,
    Item::ArrowUpgrade5,
    Item::ArrowUpgrade5,

    Item::ArrowUpgrade10,

    Item::Arrow,

    Item::TenArrows,
    Item::TenArrows,
    Item::TenArrows,
    Item::TenArrows,
    Item::TenArrows,

    Item::ThreeBombs,
    Item::ThreeBombs,
    Item::ThreeBombs,
    Item::ThreeBombs,
    Item::ThreeBombs,
    Item::ThreeBombs,
    Item::ThreeBombs,
    Item::ThreeBombs,
    Item::ThreeBombs,
    Item::ThreeBombs,

    Item::OneRupee,
    Item::OneRupee,

    Item::FiveRupees,
    Item::FiveRupees,
    Item::FiveRupees,
    Item::FiveRupees,

    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,
    Item::TwentyRupees,

    Item::FiftyRupees,
    Item::FiftyRupees,
    Item::FiftyRupees,
    Item::FiftyRupees,
    Item::FiftyRupees,
    Item::FiftyRupees,
    Item::FiftyRupees,

    Item::OneHundredRupees,

    Item::ThreeHundredRupees,
    Item::ThreeHundredRupees,
    Item::ThreeHundredRupees,
    Item::ThreeHundredRupees,
    Item::ThreeHundredRupees,
};

extern int INITIAL_UNPLACED[(int)Item::NUM_ITEMS];

void count_items();
#endif
