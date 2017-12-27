"""Partial grammar for PHP."""

from __future__ import print_function

import re

import pyparsing as p


class Error(Exception):
  pass


s = p.Suppress
G = p.Group


def ReduceToOne(prs):
  assert len(prs) == 1, 'ReduceToOne had more than one'
  assert len(list(prs[0].keys())) == len(list(
      prs[0].values())), 'Mixing labeled and unlabeled elements'
  return {key: value for key, value in prs[0].items()}


def H(pe):
  return p.Group(pe).setParseAction(ReduceToOne)


def AssertNoKeys(prs):
  keys = list(prs.keys())
  assert not keys, '{} had keys: {}'.format(prs.getName(), keys)
  return prs


def M(pe):
  return p.Group(pe).setParseAction(AssertNoKeys)

p.ParserElement.enablePackrat()
p.quotedString.addParseAction(p.removeQuotes)

boolean = p.oneOf('true false').setName('boolean')
item_in_locations = G(
    s('$locations->itemInLocations(Item::get(') + p.quotedString('item') +
    s('), [') + G(p.quotedString + p.ZeroOrMore(s(',') + p.quotedString) +
                  s(p.Optional(',')))('allowable_locations') + s('])')).setName(
                      'item_in_location')

# Lookup into item count array.
has_item = G(
    s(p.oneOf('$items $this') + '->has(') + p.quotedString('item') +
    p.Optional(s(',') + p.Word(p.nums)('count')) + s(')')).setName('has_item')
# Lookup into placement array.
locations_has = G(
    s('$locations[') + p.quotedString('location') + s(']->hasItem(Item::get(') +
    p.quotedString('item') + s('))')).setName('location_has')

# Generated global functions (or macros).
region_can_enter = G((
    (s('$this->world->getRegion(') + p.quotedString('region') + s(')')) |
    p.Literal('$this')('region')) + s('->canEnter($locations, $items)')
                    ).setName('region_can_enter')
method_call = (s(p.oneOf('$items $this') + '->') + p.Word(p.alphas) +
               s('()')).setName('method_call')
can_enter_or_complete = (
    s('$this->') + p.oneOf('can_enter can_complete')).setName('region_method')

# Reduced to true/false under assumption of open mode.
game_mode = (p.Literal("config('game-mode') == 'swordless'").setParseAction(
    p.replaceWith('false')) |
             p.Literal("in_array(config('game-mode'), ['open', 'swordless'])")
             .setParseAction(p.replaceWith('true')))
# Reduce to true/false with generation-time config options.
world_config = G(
    s('$this->world->config(') + p.quotedString('option') + s(',') +
    boolean('default') + s(')')).setName('config option')

# Item classification: Group items into sets in source code and do class lookup.
item_is_a = (
    (s('is_a($item, Item\\') - p.Word(p.alphas) - s('::class)')) |
    (s('$item instanceof Item\\') - p.Word(p.alphas))).setName('item_is_a')
item_ref = s('Item::get(') - p.quotedString - s(')')
item_is_not = s('$item !=') - item_ref
item_is_one_of = ((s('in_array($item, [') - G(
    item_ref + p.OneOrMore(s(',') - item_ref) + p.Optional(s(','))) - s('])')) |
                  (s('$item ==') - item_ref))
location_def = ((s('$this->locations[') - p.quotedString - s(']')) |
                p.Literal('$this->prize_location').setParseAction(
                    p.replaceWith('Prize')))
can_access = G(location_def('location') + s('->canAccess($items)'))

php_expr = p.Forward().setName('expression')
php_block = p.Forward().setName('block')
php_lambda = G(
    can_enter_or_complete('region_method_call') |
    ((s('function($locations, $items)') |
      s('function($item, $locations, $items)')
     ).setName('func_head') - php_block('body'))).setName('lambda')

parenthesized_expr = (s('(') + php_expr + s(')')).setName('parenthesized')
php_atom = G(
    has_item('has') | locations_has('lhas') |
    region_can_enter('access_to_region') | can_access('access') |
    method_call('method') | item_in_locations('iil') | game_mode('mode') |
    item_is_a('item_is_a') | item_is_not('is_not') |
    item_is_one_of('item_is_one_of') | world_config('config') |
    boolean('boolean') | php_lambda('lambda') |
    parenthesized_expr).setName('atom')
php_negation = (s('!') + php_atom).setName('not')
php_literal = G(php_negation('not')) | php_atom
php_and = G(php_literal + p.OneOrMore(s('&&') + php_literal)).setName('and')
php_clause = php_and('and') | php_literal
php_or = G(php_clause + p.OneOrMore(s('||') + php_clause)).setName('or')
php_term = php_or('or') | php_clause
php_ternary = G(php_term + s('?') + php_term + s(':') + php_term).setName(
    'ternary')
php_expr <<= G(php_ternary('ternary') | php_term).setName('expression')

# These can be nearly copied over nearly in tact.
return_stmt = (
    s('return') - php_expr('return') - s(';')).setName('return statement')
if_stmt = G(s('if (') - php_expr('cond') - s(')') + php_block('body')).setName(
    'if statement')
php_stmt = G(return_stmt | if_stmt('if')).setName('statement')
php_block <<= s('{') + G(p.OneOrMore(php_stmt)) + s('}')

set_requirements = s('->setRequirements(') - php_lambda('requirements') - s(')')
set_fill_rules = s('->setFillRules(') - php_lambda('fill_rules') - s(')')
rider = (set_fill_rules | set_requirements).setName('rider')
definition = (
    (G(location_def('location') - G(p.OneOrMore(rider))('riders')) |
     G(can_enter_or_complete('region_method') - s('=') - php_lambda('rhs')) |
     s('return $this')) - s(';')).ignore('// ' + p.restOfLine)
init_no_major_glitches = (s('public function initNoMajorGlitches() {') +
                          G(p.OneOrMore(definition))('definitions') + s('}'))
meta_method = (s('public function ') +
               p.Combine(p.oneOf('can has') + p.Word(p.alphas))('name') +
               s('()') + php_block('body'))


def ToTupleList(parse_results):
  if isinstance(parse_results, str):
    return parse_results

  keys = list(parse_results.keys())
  if keys:
    return {key: ToTupleList(parse_results[key]) for key in keys}
  else:
    return [ToTupleList(result) for result in parse_results]


def GetItemCollectionMethods():
  item_collection = open('Support/ItemCollection.php').read()
  methods = {}

  for result in meta_method.searchString(item_collection):
    b = ToTupleList(result)
    methods[result['name']] = b['body']
  return methods


def Smoosh(string):
  string = re.sub(r'[^a-z0-9]', '', string, flags=re.I)
  string = re.sub(r'([a-z])Of([A-Z])', r'\1of\2', string)
  return string


methods = GetItemCollectionMethods()

region_name_mapping = {
    'EastDeathMountain': 'DeathMountainEast',
    'EastDarkWorldDeathMountain': 'DarkWorldDeathMountainEast',
}


def ExpandToC(d):
  if not d:
    return
  if isinstance(d, list) and len(d) == 1:
    for x in ExpandToC(d[0]):
      yield x
    return
  if len(d) == 1:
    name, value = next(iter(d.items()))
    if name in ('lambda', 'body'):
      for statement in value:
        yield ' '.join(ExpandToC(statement))
    elif name in ('mode', 'boolean'):
      yield value
    elif name == 'return':
      yield 'return'
      for token in ExpandToC(value):
        yield token
      yield ';'
    elif name == 'method':
      yield '(' + ' '.join(ExpandToC(methods[value[0]][0][0]['return'])) + ')'
    elif name == 'region_method_call':
      yield 'return this->{method_name[0]}(Region::{region});'.format(**value)
    elif name == 'has':
      yield 'this->num_reachable(Item::{}) >= {}'.format(
          value['item'], value.get('count', 1))
    elif name == 'and':
      yield '(' + ' && '.join(
          ' '.join(str(e) for e in ExpandToC(clause)) for clause in value) + ')'
    elif name == 'or':
      yield '(' + ' || '.join(
          ' '.join(str(e) for e in ExpandToC(term)) for term in value) + ')'
    elif name == 'not':
      yield '!({})'.format(' '.join(ExpandToC(value)))
    elif name == 'ternary':
      yield '({condition}) ? ({true}) : ({false})'.format(
          condition=' '.join(ExpandToC(value[0])),
          true=' '.join(ExpandToC(value[1])),
          false=' '.join(ExpandToC(value[2])))
    elif name == 'if':
      yield 'if ({})'.format(' '.join(ExpandToC(value['cond'])))
      yield '{' + ' '.join(ExpandToC(value['body'])) + '}'
    elif name == 'access_to_region':
      if value['region'] == '$this':
        yield 'true'
      else:
        yield 'this->can_enter({})'.format('Region::' + region_name_mapping.get(
            Smoosh(value['region']), Smoosh(value['region'])))
    elif name == 'access':
      yield 'this->can_reach(Location::{})'.format(Smoosh(value['location']))
    elif name == 'lhas':
      yield 'assignments[(int)Location::{location}] == Item::{item}'.format(
          location=Smoosh(value['location']), item=Smoosh(value['item']))
    elif name == 'iil':
      yield '('
      for location in value['allowable_locations']:
        yield 'assignments[(int)Location::{location}] == {item}'.format(
            location=Smoosh(location), item='Item::' + Smoosh(value['item']))
        yield '&&'
      yield 'false)'
    elif name == 'config':
      yield value['default']
    elif name == 'is_not':
      yield 'item != Item::{}'.format(value[0])
    elif name == 'item_is_one_of':
      if isinstance(value, str):
        value = [value]
      yield '(' + '||'.join('item == Item::' + option for option in value) + ')'
    elif name == 'item_is_a':
      yield 'false'  # Massive shortcut!
    else:
      raise Error('Unhandled case: {}. Next level: {}'.format(name, value))
  else:
    import pdb
    pdb.set_trace()
