"""Build an index for ALTTP project."""

from __future__ import print_function
import collections
import pprint
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


php_expr = p.Forward()
p.quotedString.addParseAction(p.removeQuotes)

boolean = p.Literal('true') | 'false'
item_in_locations = p.Group(
    s('$locations->itemInLocations(Item::get(') + p.quotedString('item') +
    s('), [') + p.Group(p.quotedString + p.ZeroOrMore(',' + p.quotedString) +
                        p.Optional(','))('allowable_locations') +
    s('])')).setName('i_in_l')

has_item = (
    s('$items->has(') + p.quotedString +
    p.Optional(s(',') + p.Word(p.nums)('count')) + s(')')).setName('has_item')
locations_has = p.Group(
    s('$locations[') + p.quotedString('location') + s(']->hasItem(Item::get(') +
    p.quotedString('item') + s('))')).setName('location_has')
region_can_enter = (
    s('$this->world->getRegion(') + p.quotedString('region') +
    s(')->canEnter($locations, $items)')).setName('region_can_enter')
method_call = (
    s('$items->') + p.Word(p.alphas) + s('()')).setName('method_call')

swordless = p.Literal("config('game-mode') == 'swordless'").setName('swordless')
open_or_swordless = p.Literal(
    "in_array(config('game-mode'), ['open', 'swordless'])").setName(
        'open/swordless')

is_a = (
    s('is_a($item, Item\\') + p.Word(p.alphas) + s('::class)')).setName('is_a')
world_config = p.Group(
    s('$this->world->config(') + p.quotedString('option') + s(',') +
    boolean('value') + s(')')).setName('config option')

php_block = p.Forward().setName('block')
php_lambda = (s('function($locations, $items)') + php_block).setName('lambda')
parenthesized_expr = (s('(') + php_expr + s(')')).setName('parenthesized')
php_atom = (parenthesized_expr | has_item | locations_has | region_can_enter |
            method_call | item_in_locations | swordless | open_or_swordless |
            is_a | world_config | '$this->can_complete' | php_lambda |
            boolean).setName('atom')
php_negation = ('!' + php_atom).setName('negation')
php_literal = (php_negation('not') | php_atom).setName('literal')
php_and = p.Group(php_literal + p.OneOrMore(s('&&') + php_literal))('and_')
php_clause = php_literal | php_and
php_or = p.Group(php_clause + p.OneOrMore(s('||') + php_clause))('or_')

php_term = php_clause | php_or
php_ternary = p.Group(php_term + s('?') + php_term + s(':') +
                      php_term).setResultsName('ternary')
php_expr <<= ((php_term | php_ternary) + p.StringEnd()).setName('expression')

php_return = s('return') + php_expr + s(';')
if_stmt = p.Group(s('if (') + php_expr + s(')') + php_block).setName(
    'if_statment')
php_stmt = (php_return | if_stmt).setName('statement')
php_block <<= s('{') + p.OneOrMore(php_stmt) + s('}')


def BuildIndex():
  """Parse some PHP."""
  errors = []
  index = collections.defaultdict(set)
  for source, region in WalkSources():
    for location in open('locations.txt'):
      location = location.strip()
      pattern = re.compile(r'\["({})"\]->setRequirements\((.*)'.format(
          re.escape(location)), re.DOTALL | re.MULTILINE)

      source2 = re.search(r'initNoMajorGlitches.*?/\*\*', source,
                          re.DOTALL | re.MULTILINE)

      if not source2:
        continue
      source3 = source2.group(0)
      match = pattern.search(source3)
      if match:
        found_location = match.group(1)
        body = match.group(2)
        try:
          print(region, found_location)
          ether = php_expr.parseString(body)
          if 'Ether' in found_location:
            pprint.pprint(ether.asDict())
            return
        except p.ParseException as pe:
          errors.append((found_location, body, pe))

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
  _, columns = os.popen('stty size', 'r').read().split()
  for e in errors:
    print('{:<20}\t{}'.format(e[0][:20],
                              re.sub(r'\s+', ' ', e[1][:int(columns) - 20])))
  if errors:
    location, body, pe = errors[0]
    print(location)
    lines = body.splitlines()
    print('\n'.join(lines[:pe.lineno]))
    print(' ' * pe.col + '^')
    print(pe)
    print('\n'.join(lines[pe.lineno:pe.lineno + 3]))
  return index


end = BuildIndex()
