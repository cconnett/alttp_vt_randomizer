"""Partial grammar for PHP."""

import pprint
import re
import traceback

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
    s('$items->has(') + p.quotedString('item') +
    p.Optional(s(',') + p.Word(p.nums)('count')) + s(')')).setName('has_item')
# Lookup into placement array.
locations_has = G(
    s('$locations[') + p.quotedString('location') + s(']->hasItem(Item::get(') +
    p.quotedString('item') + s('))')).setName('location_has')

# Generated global functions (or macros).
region_can_enter = (
    s('$this->world->getRegion(') + p.quotedString +
    s(')->canEnter($locations, $items)')).setName('region_can_enter')
method_call = (
    s('$items->') + p.Word(p.alphas) + s('()')).setName('method_call')
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

php_expr = p.Forward()
php_block = p.Forward()
php_lambda = (G(can_complete('boss')) |
              (s('function($locations, $items)') + php_block('body')))
parenthesized_expr = s('(') + php_expr + s(')')
php_atom = G(
    has_item('has') | locations_has('lhas') | region_can_enter('region') |
    method_call('method') | item_in_locations('iil') | game_mode('mode') |
    is_a('is_a') | world_config('config') | boolean('boolean') |
    php_lambda('lambda') | parenthesized_expr)
php_negation = '!' + php_atom
php_literal = php_negation('not') | php_atom
php_and = G(php_literal + p.OneOrMore(s('&&') + php_literal))
php_clause = php_and('and') | php_literal
php_or = G(php_clause + p.OneOrMore(s('||') + php_clause))
php_term = php_or('or') | php_clause
php_ternary = G(php_term + s('?') + php_term + s(':') + php_term)
php_expr <<= G(php_ternary('ternary') | php_term)

# These can be nearly copied over nearly in tact.
return_stmt = s('return') + php_expr('return') + s(';')
if_stmt = s('if (') + php_expr + s(')') + php_block
php_stmt = return_stmt | if_stmt
php_block <<= s('{') + G(p.OneOrMore(php_stmt)) + s('}')


def Smoosh(s):
  return re.sub(r'[^a-z]', '', s, flags=re.I)

def ToTupleList(pr):
  if isinstance(pr, str):
    return pr

  keys = list(pr.keys())
  if len(keys) > 1:
    return {key: ToTupleList(pr[key]) for key in keys}
  elif keys:
    return (keys[0], ToTupleList(pr[keys[0]]))
  elif len(pr) == 1:
    return ToTupleList(pr[0])
  else:
    return [ToTupleList(e) for e in pr]


def ExpandToC(tup):
  try:
    name, value = tup
  except:
    import pdb
    pdb.set_trace()
  if name == 'lambda':
    yield from ExpandToC(value)
  elif name == 'body':
    for statement in value:
      yield from ExpandToC(statement)
  elif name == 'return':
    yield 'return'
    yield from ExpandToC(value)
    yield ';'
  elif name == 'method':
    yield value + '()'
  elif name == 'has':
    if isinstance(value, tuple):
      _, item_name = value
      yield '(world.num_reachable(Items::{}) >= 1)'.format(item_name)
    else:
      yield '(world.num_reachable(Items::{}) >= {})'.format(
          value['item'], value['count'])
  elif name == 'and':
    yield ' && '.join(
        ' '.join(str(e) for e in ExpandToC(clause)) for clause in value)
  elif name == 'or':
    yield ' || '.join(
        ' '.join(str(e) for e in ExpandToC(term)) for term in value)
  elif name == 'ternary':
    yield '({0[0]}) ? ({0[1]}) : ({0[2]})'.format(value)
  elif name == 'boss':
    yield 'return this->can_complete();'
  elif name == 'region':
    yield 'can_access_{}()'.format(Smoosh(value))
  elif name == 'mode':
    yield value
  elif name == 'lhas':
    yield 'assignments[{0[location]}] == Items::{0[item]}'.format(value)
  elif name == 'iil':
    yield 'item_in_locations({}, [{}])'.format(value['item'], ', '.join(
        Smoosh(l) for l in value['allowable_locations']))
  else:
    raise Error('Unhandled case: {}. Next level: {}'.format(name, value))
