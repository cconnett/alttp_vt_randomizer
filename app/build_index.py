"""Build an index for ALTTP project."""

from __future__ import print_function
import collections
import os
import pprint
import re

import pyparsing as p
s = p.Suppress


def GetMentionedThings(location_pattern, things, text):
  match = re.search(location_pattern, text)
  if match:
    matched_text = match.group(2)
    for item in things:
      if item in matched_text:
        yield item


def WalkSources():
  for root, _, files in os.walk('Region/'):
    for filename in files:
      path = os.path.join(root, filename)
      if path.endswith('.php'):
        yield (open(path).read(), path[:-len('.php')])


php_expr = p.Forward().setName('expr')
p.quotedString.addParseAction(p.removeQuotes)
item_in_locations = (
    s('$locations->itemInLocations(Item::get(') +
    p.quotedString.copy().setResultsName('item') + s(')') + s(',') + s('[') +
    p.Group(p.quotedString + p.ZeroOrMore(',' + p.quotedString) + p.Optional(
        ',')).setResultsName('allowable_locations') + s(']') + s(')'))
php_atom = (
    (s('$items->has(') + p.quotedString + p.Optional(s(',') + p.Word(p.nums)) +
     s(')')).setResultsName('has') |
    (s('$this->world->getRegion(') + p.quotedString +
     s(')->canEnter($locations, $items)')).setResultsName('region') |
    (s('$items->') + p.Word(p.alphas) + s('()')).setResultsName('function') |
    item_in_locations | (s('(') + php_expr + s(')')))
php_literal = p.Group(p.Optional('!') + php_atom).setResultsName('literal')
php_and = (php_literal + p.ZeroOrMore('&&' + php_literal))
php_or = (php_and + p.ZeroOrMore('||' + php_and)).setResultsName('ored')

php_expr <<= php_or
php_return = s('return') + php_expr + s(';')


def BuildIndex():
  """Parse some PHP."""
  errors = []
  index = collections.defaultdict(set)
  for source, region in WalkSources():
    for location in open('locations.txt'):
      location = location.strip()
      pattern = re.compile(
          r'\["({})"\]->setRequirements\(.*?\{{(.*?)\}}'.format(
              re.escape(location)), re.DOTALL | re.MULTILINE)
      match = pattern.search(source)
      if match:
        found_location = match.group(1)
        body = match.group(2)
        try:
          print(region, found_location)
          print(php_return.parseString(body))
        except p.ParseException:
          errors.append((found_location, body))

      # queue = collections.deque(
      #     GetMentionedThings(pattern, all_items + list(compound_items.keys()),
      #                        source))
      # done = set()
      # while queue:
      #   item = queue.pop()
      #   if item in all_items:
      #     index[item].add(location)
      #   else:
      #     if item not in done:
      #       queue.extendleft(compound_items[item])
      #   done.add(item)
  print(len(errors))
  for e in errors:
    print(e[0], e[1])
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
