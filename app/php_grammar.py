"""Partial grammar for PHP."""

from __future__ import print_function

import re

import pyparsing as p


class Error(Exception):
  pass


G = p.Group
opt = p.Optional
s = p.Suppress




def DelimitedList(token, delimiter=','):
  return token + p.ZeroOrMore(s(delimiter) + token) - s(opt(delimiter))


p.ParserElement.enablePackrat()
p.quotedString.addParseAction(p.removeQuotes)

# Common structures. These are not in groups and often appear as subexpressions.
boolean = p.oneOf('true false').setName('boolean').setParseAction(
    lambda pr: pr[0] == 'true')
integer = p.Word(
    p.nums).setName('integer').setParseAction(lambda pr: int(pr[0]))
identifier = p.Word(p.alphas + '_', p.alphanums + '_').setName('identifier')
location_def = ((s('$this->locations[') - p.quotedString - s(']')) |
                p.Literal('$this->prize_location').setParseAction(
                    p.replaceWith('Prize')))
# Reduced to true/false under assumption of open mode.
game_mode = ((p.Literal("config('game-mode') == 'swordless'").setParseAction(
    p.replaceWith({
        'boolean': False
    })) | p.Literal("in_array(config('game-mode'), ['open', 'swordless'])")
              .setParseAction(p.replaceWith({
                  'boolean': True
              }))))

# Item classification: Group items into sets in source code and do class lookup.
item_is_a = (('is_a($item, Item\\' - identifier - '::class)') |
             ('$item instanceof Item\\' - identifier)).setName('item_is_a')
item_ref = s('Item::get(') - p.quotedString - s(')')
item_is_not = G('$item !=' - item_ref('item'))
item_is = G(('in_array($item, [' - G(DelimitedList(item_ref))('items') - '])') |
            ('$item ==' - G(item_ref)('items')))

# End of ungrouped structures.

# Reduce to true/false with generation-time config options.
world_config = G('$this->world->config(' - p.quotedString('option') - ',' - (
    boolean | integer)('default') - ')').setName('config expression')

# Top-level expression alternates. These should be in groups so they get
# substructures.
item_in_locations = G('$locations->itemInLocations(Item::get(' + p.quotedString(
    'item') + '), [' + DelimitedList(p.quotedString)('allowable_locations') +
                      '])').setName('item_in_location')

# Lookup into item count array.
has_item = G(
    p.oneOf('$items $this') + '->has(' + p.quotedString('item') +
    opt(',' + integer('count')) + ')').setName('has_item')
# Lookup into placement array.
location_has = G(
    '$locations[' + p.quotedString('location') + ']->hasItem(Item::get(' +
    p.quotedString('item') + '))').setName('location_has')

can_access = G(location_def('location') + '->canAccess($items)')

# Generated global functions (or macros).
region_can_enter = G((
    ('$this->world->getRegion(' + p.quotedString('region') + ')') |
    (p.Literal('$this')('region'))) + '->canEnter($locations, $items)').setName(
        'region_can_enter')
method_call = G(
    p.oneOf('$items $this') + '->' + identifier('method_name') + '()').setName(
        'call to method of ItemCollection')
can_enter_or_complete = G('$this->' + p.oneOf('can_enter can_complete')
                          ('method_name')).setName('region_method')

php_expr = p.Forward().setName('expression')
php_block = p.Forward().setName('block')
php_callable_expression = G(
    can_enter_or_complete('call_to_region_method') | (p.oneOf([
        'function($locations, $items)',
        'function($item, $locations, $items)',
        'function($item, $items)',
    ]) - php_block('body'))).setName('callable')

parenthesized_expr = (s('(') + php_expr + s(')')).setName('parens')
php_atom = (G(
    has_item('has') | location_has('location_has_item') |
    region_can_enter('access_to_region') | can_access('access_to_location') |
    method_call('call_builtin') | item_in_locations('item_in_locations') |
    game_mode('mode') | item_is_a('item_is_a') | item_is_not('item_is_not') |
    item_is('item_is') | world_config('config') | boolean('boolean') |
    php_callable_expression('callable')) | parenthesized_expr).setName('atom')
php_negation = G('!' + php_atom('not')).setName('negation')
php_literal = php_negation | php_atom
php_and = G(php_literal + p.OneOrMore(s('&&') - php_literal)).setName('and')
php_clause = G(php_and('and')) | php_literal
php_or = G(php_clause + p.OneOrMore(s('||') - php_literal)).setName('or')
php_term = G(php_or('or')) | php_clause
php_ternary = G(
    php_term('if') + '?' - php_term('then') - ':' - php_term('else')).setName(
        'ternary')
php_expr <<= (G(php_ternary('ternary')) | php_term).setName('expression')

# These can be nearly copied over nearly in tact.
return_stmt = G('return' - php_expr('return') - ';').setName('return statement')
if_stmt = G('if (' - php_expr('condition') - ')' - php_block('body')).setName(
    'if statement')
php_stmt = (return_stmt | G(if_stmt('if'))).setName('statement')
php_block <<= s('{') + G(p.OneOrMore(php_stmt)) + s('}')

# The logic is defined by calling methods on Location objects to specify
# requirements (items needed to access the location), fill rules (a predicate
# taking an item and reporting if it is allowed to be placed there), and
# always-allow rules (a predicate taking an item and reporting if that item is
# whitelisted to be placed there regardless of access requirements and fill
# rules).
#
# An example to explain always-allow: The v28 key in its own chest is done with
# always-allow, because the always allow predicate set on such a chest returns
# true for the key.

set_predicate = G('->' + p.oneOf('setRequirements setFillRules setAlwaysAllow')
                  ('slot') - '(' - php_callable_expression('function') - ')'
                 ).setName('location_predicate')

location_definition = G(
    location_def('location') - p.OneOrMore(set_predicate)('predicates'))
region_method_definition = G(
    can_enter_or_complete('region_method') + '=' -
    php_callable_expression('rhs'))
definition = G((location_definition('location_definition') |
                region_method_definition('region_method_definition')) - ';'
              ).ignore('// ' + p.restOfLine)

init_no_major_glitches = G(
    'public function initNoMajorGlitches() {' - p.OneOrMore(definition)
    ('definitions') - 'return $this;' - '}')
meta_method = G('public function ' + p.Combine(p.oneOf('can has') + identifier)
                ('name') + '()' - php_block('body'))


def GetItemCollectionMethods():
  item_collection = open('Support/ItemCollection.php').read()
  ret = {}

  for result in meta_method.searchString(item_collection):
    result = result[0].asDict()
    ret[result['name']] = result['body'][0]
  return ret


def Smoosh(string):
  string = re.sub(r'[^a-z0-9]', '', string, flags=re.I)
  string = re.sub(r'([a-z])Of([A-Z])', r'\1of\2', string)
  return string


methods = GetItemCollectionMethods()

region_name_mapping = {
    'EastDeathMountain': 'DeathMountainEast',
    'WestDeathMountain': 'DeathMountainWest',
    'EastDarkWorldDeathMountain': 'DarkWorldDeathMountainEast',
    'WestDarkWorldDeathMountain': 'DarkWorldDeathMountainWest',
    'Mire': 'DarkWorldMire',
    'SouthDarkWorld': 'DarkWorldSouth',
    'NorthEastDarkWorld': 'DarkWorldNorthEast',
    'NorthWestDarkWorld': 'DarkWorldNorthWest',
}


def ExpandToC(d):
  name, value = next(iter(d.items()))
  # General C constructs.
  if name == 'return':
    return 'return ({});'.format(ExpandToC(value))
  elif name in ('body',):
    return '\n'.join(ExpandToC(statement) for statement in value)
  elif name == 'and':
    return '({})'.format(' && '.join(ExpandToC(clause) for clause in value))
  elif name == 'or':
    return '({})'.format(' || '.join(ExpandToC(term) for term in value))
  elif name == 'not':
    return '!({})'.format(ExpandToC(value))
  elif name == 'if':
    return 'if ({condition}) {{ {body} }} '.format(
        condition=ExpandToC(value['condition']),
        body='\n'.join(ExpandToC(stmt) for stmt in value['body']))
  elif name == 'ternary':
    return '(({condition}) ? ({true}) : ({false}))'.format(
        condition=ExpandToC(value['if']),
        true=ExpandToC(value['then']),
        false=ExpandToC(value['else']))
  elif name == 'boolean':
    return 'true' if value else 'false'
  # ALTTP specifics
  elif name == 'call_to_region_method':
    return 'return this->{method_name}(Region::{region});'.format(**value)
  elif name == 'call_builtin':
    return ExpandToC(methods[value['method_name']][0]['return'])
  elif name == 'has':
    return 'this->num_reachable(Item::{}) >= {}'.format(value['item'],
                                                        value.get('count', 1))
  elif name == 'location_has_item':
    return 'assignments[(int)Location::{location}] == Item::{item}'.format(
        location=Smoosh(value['location']), item=Smoosh(value['item']))
  elif name == 'item_in_locations':
    return '(' + ' || '.join(
        'assignments[(int)Location::{location}] == {item}'.format(
            location=Smoosh(location), item='Item::' + Smoosh(value['item']))
        for location in value['allowable_locations']) + ')'
  elif name == 'access_to_region':
    if value['region'] == '$this':
      return 'true'
    else:
      return 'this->can_enter({})'.format('Region::' + region_name_mapping.get(
          Smoosh(value['region']), Smoosh(value['region'])))
  elif name == 'access_to_location':
    return 'this->can_reach(Location::{})'.format(Smoosh(value['location']))
  elif name == 'config':
    return ExpandToC({'boolean': value['default']})
  elif name == 'mode':
    return ExpandToC(value)
  elif name == 'item_is':
    return '({})'.format(' || '.join(
        'item == Item::' + option for option in value['items']))
  elif name == 'item_is_not':
    return 'item != Item::{}'.format(value['item'][0])
  elif name == 'item_is_a':
    return 'false'  # Massive shortcut!
  else:
    raise Error('Unhandled case: {}. Next level: {}'.format(name, value))
