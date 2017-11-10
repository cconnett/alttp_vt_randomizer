"""Build an index for ALTTP project."""

from __future__ import print_function
import collections
import os
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

items_has = (s('$items->has(') + p.quotedString +
             p.Optional(s(',') + p.Word(p.nums)) + s(')')).setResultsName('has')
region_can_enter = (
    s('$this->world->getRegion(') + p.quotedString +
    s(')->canEnter($locations, $items)')).setResultsName('region')
function_call = (
    s('$items->') + p.Word(p.alphas) + s('()')).setResultsName('function')
swordless = p.Literal("config('game-mode') == 'swordless'")
open_or_swordless = "in_array(config('game-mode'), ['open', 'swordless'])"

is_a = 'is_a($item, Item\\' + p.Word(p.alphas) + '::class)'
boss_normal_location = p.Literal(
    "!$this->world->config('region.bossNormalLocation', true)")
parenthesized_expr = (s('(') + php_expr + s(')'))
php_atom = (items_has | region_can_enter | function_call | item_in_locations |
            swordless | open_or_swordless | is_a | boss_normal_location |
            parenthesized_expr)
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
  print('===Errors===')
  print(len(errors))
  for e in errors:
    print(e[0], e[1])
  return index

end = BuildIndex()
