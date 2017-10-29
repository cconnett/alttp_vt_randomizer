"""Build an index for ALTTP project."""

import collections
import os
import pprint
import re
all_items = [item_line.strip() for item_line in open('items.txt').readlines()]


def GetMentionedThings(location_pattern, things, text):
  match = re.search(location_pattern, text)
  if match:
    matched_text = match.group(1)
    for item in things:
      if item in matched_text:
        yield item


def WalkSources():
  for root, _, files in os.walk('Region/'):
    for filename in files:
      if filename.endswith('.php'):
        yield open(os.path.join(root, filename)).read()


def BuildIndex():
  index = collections.defaultdict(set)
  for source in WalkSources():
    for location in open('locations.txt'):
      location = location.strip()
      pattern = re.compile(r'\["{}"\]->setRequirements\(.*?\{{(.*?)\}}'.format(
          re.escape(location)), re.DOTALL | re.MULTILINE)
      queue = collections.deque(
          GetMentionedThings(pattern, all_items + list(compound_items.keys()),
                             source))
      done = set()
      while queue:
        item = queue.pop()
        if item in all_items:
          index[item].add(location)
        else:
          if item not in done:
            queue.extendleft(compound_items[item])
        done.add(item)
  return index


compound_items = {
    'Desert Palace': [
        'BookOfMudora', 'MagicMirror', 'canLiftDarkRocks', 'canFly'
    ],
    'East Dark World Death Mountain': [
        'canLiftDarkRocks',
        'East Death Mountain',
    ],
    'East Death Mountain': [
        'West Death Mountain', 'Hammer', 'MagicMirror', 'Hookshot'
    ],
    'Ganons Tower': [
        'MoonPearl', 'Crystal1', 'Crystal2', 'Crystal3', 'Crystal4', 'Crystal5',
        'Crystal6', 'Crystal7', 'East Dark World Death Mountain'
    ],
    'Mire': ['canFly', 'canLiftDarkRocks'],
    'Misery Mire': [
        'Bombos', 'Ether', 'Quake', 'MoonPearl', 'PegasusBoots', 'Hookshot',
        'Mire', 'hasSword'
    ],
    'North East Dark World': [
        'DefeatAgahnim', 'Hammer', 'canLiftRocks', 'MoonPearl',
        'canLiftDarkRocks', 'Flippers'
    ],
    'North West Dark World': [
        'North East Dark World', 'Hookshot', 'Flippers', 'Hammer',
        'canLiftRocks', 'canLiftDarkRocks', 'MoonPearl'
    ],
    'South Dark World': [
        'MoonPearl', 'canLiftDarkRocks', 'Hammer', 'canLiftRocks',
        'DefeatAgahnim', 'Hookshot', 'Flippers', 'MagicMirror', 'PegasusBoots'
    ],
    'Tower of Hera': ['Mirror', 'Hookshot', 'Hammer', 'West Death Mountain'],
    'Turtle Rock': [
        'Bombos', 'Ether', 'Quake', 'hasSword', 'MoonPearl', 'canLiftDarkRocks',
        'Hammer', 'East Death Mountain', 'CaneOfSomaria'
    ],
    'West Death Mountain': ['canFly', 'canLiftRocks', 'Lamp'],
    'canLiftRocks': ['PowerGlove', 'ProgressiveGlove', 'TitansMitt'],
    'canLiftDarkRocks': ['TitansMitt', 'ProgressiveGlove'],
    'canLightTorches': ['FireRod', 'Lamp'],
    'canMeltThings': ['FireRod', 'Bombos', 'hasSword'],
    'canFly': ['OcarinaActive', 'OcarinaInactive'],
    'canSpinSpeed': ['PegasusBoots', 'hasSword', 'Hookshot'],
    'canShootArrows': ['Bow', 'BowAndArrows', 'BowAndSilverArrows'],
    'canBlockLasers': ['MirrorShield', 'ProgressiveShield'],
    'canExtendMagic': ['HalfMagic', 'QuarterMagic', 'hasABottle'],
    'glitchedLinkInDarkWorld': ['MoonPearl', 'hasABottle'],
    'hasSword': [
        'L1Sword', 'L1SwordAndShield', 'ProgressiveSword', 'hasUpgradedSword'
    ],
    'hasUpgradedSword': [
        'L2Sword', 'MasterSword', 'L3Sword', 'L4Sword', 'ProgressiveSword'
    ],
    'hasABottle': [
        'BottleWithBee', 'BottleWithFairy', 'BottleWithRedPotion',
        'BottleWithGreenPotion', 'BottleWithBluePotion', 'Bottle',
        'BottleWithGoldBee'
    ],
}

pprint.pprint({key: val for key, val in BuildIndex().items()})
