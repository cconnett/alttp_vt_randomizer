"""Partial grammar for PHP."""

from __future__ import print_function

import re

import pyparsing as p


class Error(Exception):
  pass


s = p.Suppress
G = p.Group


p.ParserElement.enablePackrat()
p.quotedString.addParseAction(p.removeQuotes)

boolean = p.oneOf('true false').setName('boolean')
item_in_locations = G(
    s('$locations->itemInLocations(Item::get(') + p.quotedString('item') +
    s('), [') + G(p.quotedString + p.ZeroOrMore(s(',') + p.quotedString) + s(
        p.Optional(',')))('allowable_locations') + s('])')).setName(
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
region_can_enter = (
    s('$this->world->getRegion(') + p.quotedString +
    s(')->canEnter($locations, $items)')).setName('region_can_enter')
method_call = (s(p.oneOf('$items $this') + '->') + p.Word(p.alphas) +
               s('()')).setName('method_call')
can_complete = p.Literal('$this->can_complete').setName('can_complete')('boss')

# Reduced to true/false under assumption of open mode.
game_mode = (p.Literal("config('game-mode') == 'swordless'").setParseAction(
    p.replaceWith('false')) |
             p.Literal("in_array(config('game-mode'), ['open', 'swordless'])")
             .setParseAction(p.replaceWith('true')))
# Reduce to true/false with generation-time config options.
world_config = G(
    s('$this->world->config(') + p.quotedString('option') + s(',') +
    boolean('value') + s(')')).setName('config option')

# Item classification: Group items into sets in source code and do class lookup.
is_a = (
    s('is_a($item, Item\\') + p.Word(p.alphas) + s('::class)')).setName('is_a')

php_expr = p.Forward().setName('expression')
php_block = p.Forward().setName('block')
php_lambda = (
    G(can_complete('boss')) |
    (s('function($locations, $items)') + php_block('body'))).setName('lambda')

parenthesized_expr = (s('(') + php_expr + s(')')).setName('parenthesized')
php_atom = G(
    has_item('has') | locations_has('lhas') | region_can_enter('region') |
    method_call('method') | item_in_locations('iil') | game_mode('mode') |
    is_a('is_a') | world_config('config') | boolean('boolean') |
    php_lambda('lambda') | parenthesized_expr).setName('atom')
php_negation = ('!' + php_atom).setName('not')
php_literal = php_negation('not') | php_atom
php_and = G(php_literal + p.OneOrMore(s('&&') + php_literal)).setName('and')
php_clause = php_and('and') | php_literal
php_or = G(php_clause + p.OneOrMore(s('||') + php_clause)).setName('or')
php_term = php_or('or') | php_clause
php_ternary = G(php_term + s('?') + php_term + s(':') + php_term).setName(
    'ternary')
php_expr <<= G(php_ternary('ternary') | php_term)

# These can be nearly copied over nearly in tact.
return_stmt = (
    s('return') + php_expr('return') + s(';')).setName('return statement')
if_stmt = (s('if (') + php_expr + s(')') + php_block).setName('if statement')
php_stmt = return_stmt | if_stmt
php_block <<= s('{') + G(p.OneOrMore(php_stmt)) + s('}')


def ToTupleList(parse_results):
  if isinstance(parse_results, str):
    return parse_results

  keys = list(parse_results.keys())
  if len(keys) > 1:
    return {key: ToTupleList(parse_results[key]) for key in keys}
  elif keys:
    return (keys[0], ToTupleList(parse_results[keys[0]]))
  elif len(parse_results) == 1:
    return ToTupleList(parse_results[0])
  else:
    return [ToTupleList(result) for result in parse_results]


item_collection = open('Support/ItemCollection.php').read()
methods = {}

for name, body in re.findall(
    r'public function ([A-Za-z]+)\(\) (\{.*?)(?:/\*\*|$)',
    item_collection,
    flags=re.DOTALL):
  try:
    return_, expr = ToTupleList(php_block.parseString(body))
    assert return_ == 'return'
    methods[name] = expr
  except p.ParseException as e:
    continue


def Smoosh(string):
  return re.sub(r'[^a-z0-9]', '', string, flags=re.I)


def ExpandToC(tup):
  name, value = tup
  if name == 'lambda':
    for token in ExpandToC(value):
      yield token
  elif name == 'body':
    for statement in value:
      for token in ExpandToC(statement):
        yield token
  elif name == 'return':
    yield 'return'
    for token in ExpandToC(value):
      yield token
    yield ';'
  elif name == 'method':
    try:
      yield '(' + ' '.join(ExpandToC(methods[value])) + ')'
    except:
      raise
  elif name == 'has':
    if isinstance(value, tuple):
      _, item_name = value
      yield 'this->num_reachable(Items::{}) >= 1'.format(item_name)
    else:
      yield 'this->num_reachable(Items::{}) >= {}'.format(
          value['item'], value['count'])
  elif name == 'and':
    yield '(' + ' && '.join(' '.join(str(e) for e in ExpandToC(clause))
                            for clause in value) + ')'
  elif name == 'or':
    yield '(' + ' || '.join(' '.join(str(e) for e in ExpandToC(term))
                            for term in value) + ')'
  elif name == 'ternary':
    yield '({condition}) ? ({true}) : ({false})'.format(
        condition=' '.join(ExpandToC(value[0])),
        true=' '.join(ExpandToC(value[1])),
        false=' '.join(ExpandToC(value[2])))
  elif name == 'boss':
    pass
    # yield 'this->can_complete({})'.format('Locations::Regions::' +
    #                                       Smoosh(value))
  elif name == 'region':
    yield 'this->can_enter({})'.format('Locations::Regions::' + Smoosh(value))
  elif name == 'mode':
    yield value
  elif name == 'lhas':
    yield 'assignments[Locations::{location}] == Items::{item}'.format(
        location=Smoosh(value['location']), item=Smoosh(value['item']))
  elif name == 'iil':
    yield '('
    for location in value['allowable_locations']:
      yield 'assignments[Locations::{location}] == {item}'.format(
          location=Smoosh(location), item='Items::' + Smoosh(value['item']))
      yield '&&'
    yield 'false)'
  else:
    raise Error('Unhandled case: {}. Next level: {}'.format(name, value))
