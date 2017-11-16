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


p.ParserElement.enablePackrat()
p.quotedString.addParseAction(p.removeQuotes)

boolean = p.oneOf('true false').setName('boolean')
item_in_locations = (
    s('$locations->itemInLocations(Item::get(') + p.quotedString('item') +
    s('), [') + p.Group(p.quotedString + p.ZeroOrMore(',' + p.quotedString) +
                        p.Optional(','))('allowable_locations') +
    s('])')).setName('item_in_location')

has_item = p.Group(
    s('$items->has(') + p.quotedString('item') +
    p.Optional(s(',') + p.Word(p.nums)('count')) + s(')')).setName('has_item')
locations_has = p.Group(
    s('$locations[') + p.quotedString('location') + s(']->hasItem(Item::get(') +
    p.quotedString('item') + s('))')).setName('location_has')
region_can_enter = (
    s('$this->world->getRegion(') + p.quotedString('region') +
    s(')->canEnter($locations, $items)')).setName('region_can_enter')
method_call = (
    s('$items->') + p.Word(p.alphas) + s('()')).setName('method_call')

game_mode = (p.Literal("config('game-mode') == 'swordless'").setParseAction(
    p.replaceWith('swordless')) |
             p.Literal("in_array(config('game-mode'), ['open', 'swordless'])")
             .setParseAction(p.replaceWith('open/swordless')))
can_complete = p.Literal('$this->can_complete').setName('can_complete')
is_a = (
    s('is_a($item, Item\\') + p.Word(p.alphas) + s('::class)')).setName('is_a')
world_config = p.Group(
    s('$this->world->config(') + p.quotedString('option') + s(',') +
    boolean('value') + s(')')).setName('config option')

php_expr = p.Forward().setName('expression')
php_block = p.Forward().setName('block')
php_lambda = (
    can_complete('boss') |
    (s('function($locations, $items)') + php_block('body'))).setName('lambda')
parenthesized_expr = (s('(') + php_expr + s(')')).setName('parenthesized')
php_atom = (has_item('has') | locations_has('lhas') |
            region_can_enter('region') | method_call('method') |
            item_in_locations('iil') | game_mode('game_mode') | is_a('is_a') |
            world_config('world_config') | boolean | php_lambda('lambda') |
            parenthesized_expr).setName('atom')
php_negation = ('!' + php_atom).setName('negation')
php_literal = (php_negation('not') | php_atom).setName('literal')
php_and = p.Group(php_literal + p.OneOrMore(s('&&') + php_literal)).setName(
    'and')
php_clause = php_and('and') | php_literal
php_or = p.Group(php_clause + p.OneOrMore(s('||') + php_clause)).setName('or')

php_term = php_or('or') | php_clause
php_ternary = p.Group(php_term + s('?') + php_term + s(':') + php_term).setName(
    'ternary')
php_expr <<= (php_ternary | php_term)

return_stmt = (s('return') + php_expr + s(';')).setName('return statement')
if_stmt = p.Group(
    s('if (') + php_expr('condition') + s(')') + php_block('action')).setName(
        'if_statment')
php_stmt = (return_stmt | if_stmt).setName('statement')
php_block <<= p.Group(s('{') + p.OneOrMore(php_stmt) + s('}'))

locations = [line.strip() for line in open('locations.txt')]
parsed_location_requirements = {}


def BuildIndex():
  """Parse some PHP."""
  errors = []
  index = collections.defaultdict(set)
  for location in locations:
    for source, _ in WalkSources():
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
          # print(region, found_location)
          php_expr.parseString(body)
          break
        except p.ParseException as pe:
          errors.append((found_location, body, pe))

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
