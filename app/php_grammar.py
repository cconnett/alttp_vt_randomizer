"""Partial grammar for PHP."""

from __future__ import print_function

import re

import pyparsing as p


class Error(Exception):
  pass


p.ParserElement.enablePackrat()
G = p.Group
opt = p.Optional
s = p.Suppress
LEFT = p.opAssoc.LEFT
RIGHT = p.opAssoc.RIGHT


def DelimitedList(token, delimiter=','):
  return token + p.ZeroOrMore(s(delimiter) + token) - s(opt(delimiter))


def ConvertInfixListToPrefixDict(pr):
  if not isinstance(pr, p.ParseResults):
    #print('Got non-pr:', pr)
    #print('Returning it unchanged.')
    return pr
  #print('Got', pr.dump())

  if pr.haskeys():
    result = pr.asDict()
  elif len(pr) > 1:
    if pr[0] == '!':
      result = {'!': ConvertInfixListToPrefixDict(pr[1])}
    elif pr[1] == '?' and pr[3] == ':':
      assert len(pr) == 5
      result = {
          'ternary': {
              'if': ConvertInfixListToPrefixDict(pr[0]),
              'then': ConvertInfixListToPrefixDict(pr[2]),
              'else': ConvertInfixListToPrefixDict(pr[4]),
          }
      }
    else:
      # Infix lists are fence-post `<term> (<op> <term>)+`.
      assert len(pr) % 2 == 1
      operator = pr[1]
      # All operators at this level must be equal.
      assert all(op == operator for op in pr[1::2])

      result = {
          operator: [
              ConvertInfixListToPrefixDict(subexpr) for subexpr in pr[0::2]
          ]
      }
  else:  # len(pr) == 1:
    try:
      result = ConvertInfixListToPrefixDict(pr[0])
    except Exception as e:
      print('Poo poo.')
      import pdb
      pdb.set_trace()
      print()

  if isinstance(result, dict):
    #print('Returning', result)
    return result
  else:
    #print('I should return a dict, but I am actually returning', result)
    import pdb
    pdb.set_trace()


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
              })))).setName('game mode assertion')

# Item classification: Group items into sets in source code and do class lookup.
item_is_a = (('is_a($item, Item\\' - identifier - '::class)') |
             ('$item instanceof Item\\' - identifier)).setName('item_is_a')
item_ref = (s('Item::get(') - p.quotedString - s(')')).setName('item reference')
item_is_not = G('$item !=' - item_ref('item')).setName(
    'negative item assertion')
item_is = G(('in_array($item, [' - G(DelimitedList(item_ref))('items') - '])') |
            ('$item ==' - G(item_ref)('items'))).setName('item assertion')

# End of ungrouped structures.

# Reduce to true/false with generation-time config options.
world_config = G('$this->world->config(' - p.quotedString('option') - ',' - (
    boolean | integer)('default') - ')').setName('config expression')

# Top-level expression alternates. These should be in groups so they get
# substructures.
item_in_locations = G('$locations->itemInLocations(Item::get(' - p.quotedString(
    'item') - ')' - ',' - '[' - DelimitedList(p.quotedString)(
        'allowable_locations') - ']' - ')').setName('item_in_location')

# Lookup into item count array.
has_item = G(
    p.oneOf('$items $this') + '->has(' + p.quotedString('item') +
    opt(',' + integer('count')) + ')').setName('has_item')
# Lookup into placement array.
location_has = (G('$locations[' + p.quotedString('location') +
                  ']->hasItem(Item::get(' + G(p.quotedString)('items') + '))') |
                G('in_array($locations[' - p.quotedString('location') -
                  ']->getItem(), [' - DelimitedList(item_ref)('items') - '])'))

can_access = G(location_def('location') + '->canAccess($items)').setName(
    'reachability assertion')

# Generated global functions (or macros).
region_can_enter = G((
    ('$this->world->getRegion(' + p.quotedString('region') + ')') |
    (p.Literal('$this')('region'))) + '->canEnter($locations, $items)').setName(
        'region_can_enter')
method_call = G(
    p.oneOf('$items $this') + '->' + identifier('method_name') + '(' -
    opt(integer('actual_parameter')) - ')').setName(
        'call to method of ItemCollection')
can_enter_or_complete = G('$this->' + p.oneOf('can_enter can_complete')
                          ('method_name')).setName('region_method')

php_block = p.Forward().setName('block')
php_var = G(s('$') + identifier('symbol')).setName('variable')
php_callable_expression = G(
    can_enter_or_complete('call_to_region_method') | (p.oneOf([
        'function($locations, $items)',
        'function($item, $locations, $items)',
        'function($item, $items)',
    ]) - G(php_block)('body'))).setName('callable')

php_atom = G(
    has_item('has') | location_has('location_has_item') |
    region_can_enter('access_to_region') | can_access('access_to_location') |
    method_call('call_builtin') | item_in_locations('item_in_locations') |
    game_mode('mode') | item_is_a('item_is_a') | item_is_not('item_is_not') |
    item_is('item_is') | world_config('config') | boolean('boolean') |
    integer('integer') | php_callable_expression('callable') |
    php_var('var')).setName('atom')

php_expr = p.Forward()  # Removing this Forward messes up the output types.
php_expr <<= p.infixNotation(
    php_atom,
    [
        ('!', 1, RIGHT),
        ('*', 2, LEFT),
        ('+', 2, LEFT),
        (p.oneOf('< <= == >= >'), 2, LEFT),
        ('&&', 2, LEFT),
        ('||', 2, LEFT),
        # Attention! The ternary operator is right-associative in C, but
        # left-associative in PHP! We over-parenthesize everything we emit, so
        # emitted code should match the PHP semantics.
        #
        # Example: "true ? false : false ? false : true"
        # This expression evaluates to true in PHP and false in C.
        (('?', ':'), 3, LEFT),
    ]).setParseAction(ConvertInfixListToPrefixDict)

return_stmt = G('return' - php_expr('return') - ';').setName('return statement')
if_stmt = G('if (' - php_expr('condition') - ')' - G(php_block)
            ('body')).setName('if statement')
php_stmt = (return_stmt | G(if_stmt('if'))).setName('statement')
php_block <<= s('{') + p.OneOrMore(php_stmt) + s('}')

# The logic is defined by calling methods on Location objects to specify
# "requirements" (items needed to access the location), "fill rules" (a
# predicate taking an item and reporting if it is allowed to be placed there),
# and "always-allow" rules (a predicate taking an item and reporting if that
# item is whitelisted to be placed there regardless of access requirements and
# fill rules).
#
# An example to explain always-allow: In v28, a key can be placed in its own
# chest. This is accomplished by setting an always-allow function that returns
# true for such a key. The requirements would block that placement, but
# always-allow short-circuits the requirements check.

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
php_type = p.oneOf('bool int')
parameter = (s(opt(php_type)) + php_var('parameter') + '=' - integer('default'))
meta_method = G('public function ' + p.Combine(p.oneOf('can has') + identifier)
                ('name') + '(' + opt(parameter) - ')' - opt(s(':') - php_type) -
                php_block('body'))


def GetItemCollectionMethods():
  item_collection = open('Support/ItemCollection.php').read()
  ret = {}

  try:
    method_results = meta_method.searchString(item_collection)
  except (p.ParseException, p.ParseSyntaxException) as e:
    print('Bad ItemCollection')
    print(e.lineno)
    print(e.line)
    print(' ' * (e.col - 1) + '^')
    print(e)
    import sys
    sys.exit()
  for result in method_results:
    n = result[0].asDict()
    ret[n['name']] = n
  return ret


def Smoosh(string):
  string = re.sub(r'[^a-z0-9]', '', string, flags=re.I)
  string = re.sub(r'([a-z])Of([A-Z])', r'\1of\2', string)
  return string


methods = GetItemCollectionMethods()
methods['bottleCount'] = {
    'body': [{
        'return': {
            'integer': 'this->bottle_count()'
        }
    }]
}

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
  elif name in ('&&', '||', '*', '+'):
    return '({})'.format(name.join(ExpandToC(expr) for expr in value))
  elif name == '!':
    return '!({})'.format(ExpandToC(value))
  elif name == 'if':
    return 'if ({condition}) {{ {body} }} '.format(
        condition=ExpandToC(value['condition']),
        body='\n'.join(ExpandToC(stmt) for stmt in value['body']))
  elif name in ('<', '>='):
    return '(({left}){operator}({right}))'.format(
        left=ExpandToC(value[0]), operator=name, right=ExpandToC(value[1]))
  elif name == 'ternary':
    return '(({condition}) ? ({true}) : ({false}))'.format(
        condition=ExpandToC(value['if']),
        true=ExpandToC(value['then']),
        false=ExpandToC(value['else']))
  elif name == 'boolean':
    return 'true' if value else 'false'
  # PHP constructs.
  elif name == 'var':
    return '{%s}' % 'foodiebar'
  elif name == 'integer':
    return str(value)
  # ALTTP specifics.
  elif name == 'call_to_region_method':
    return 'return this->{method_name}(Region::{region});'.format(**value)
  elif name == 'call_builtin':
    result = ExpandToC(methods[value['method_name']]['body'][0]['return'])
    foodiebar = value.get('actual_parameter',
                          methods[value['method_name']].get('default'))
    if foodiebar:
      result = result.format(foodiebar=foodiebar)
    return result
  elif name == 'has':
    return 'this->is_num_reachable({n}, Item::{item})'.format(
      item=value['item'],
      n=value.get('count', 1))
  elif name == 'location_has_item':
    return '(' + ' || '.join(
        'assignments[(int)Location::{location}] == Item::{item}'.format(
            location=Smoosh(value['location']), item=Smoosh(item))
        for item in value['items']) + ')'
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
    default = value['default']
    if isinstance(default, bool):
      return ExpandToC({'boolean': default})
    elif isinstance(default, int):
      return str(default)
    else:
      assert False
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
