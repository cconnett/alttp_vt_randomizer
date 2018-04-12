#!/usr/bin/python3.6
"""Partial grammar for PHP."""

from __future__ import print_function

import pdb
import re
import sys

import infix_notation
import pyparsing as p

current_region = [None]


class Error(Exception):
  pass


p.ParserElement.enablePackrat()
G = p.Group
opt = p.Optional
s = p.Suppress
LEFT = p.opAssoc.LEFT
RIGHT = p.opAssoc.RIGHT

config = {
    'mode.weapons': 'RANDOM',
}


def MakeConstant(string):
  """Make a string into A_CONSTANT_NAME."""
  return re.sub(r'[^\w]', '_', string).upper()


def DelimitedList(token, delimiter=','):
  return token + p.ZeroOrMore(s(delimiter) + token) - s(opt(delimiter))


def ConvertInfixListToPrefixDict(pr):
  """Convert an infix list from `infixNotation` to a prefix-notation dict."""
  if not isinstance(pr, p.ParseResults):
    # print('Got non-pr:', pr)
    # print('Returning it unchanged.')
    return pr
  # print('Got', pr.dump())

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
      _ = e
      print('Poo poo.')
      pdb.set_trace()
      print()
      raise

  if isinstance(result, dict):
    # print('Returning', result)
    return result
  else:
    # print('I should return a dict, but I am actually returning', result)
    pdb.set_trace()


p.quotedString.addParseAction(p.removeQuotes)

php_expr = p.Forward()

# Common structures. These are not in groups and often appear as subexpressions.
boolean = p.oneOf('true false').setName('boolean').setParseAction(
    lambda pr: pr[0] == 'true')
integer = p.Word(
    p.nums).setName('integer').setParseAction(lambda pr: int(pr[0]))
enum_string = (
    p.Literal("'uncle'") |
    "'swordless'").setParseAction(lambda pr: MakeConstant(pr[0].strip("'")))
identifier = p.Word(p.alphas + '_', p.alphanums + '_').setName('identifier')
location_def = ((s('$this->locations[') - p.quotedString - s(']')) |
                p.Literal('$this->prize_location').setParseAction(
                    p.replaceWith('Prize')))
item_ref = (s('Item::get(') - p.quotedString - s(')')).setName('item reference')

# Common structure that _is_ labeled.
php_var = G(s('$') + identifier('symbol')).setName('variable')

# Reduced to true/false under assumption of open mode.
game_mode_true = p.Literal("$this->world->getGoal() == 'ganon'").setParseAction(
                      p.replaceWith({
                          'boolean': True
                      }))
game_mode_false = (
    p.Literal("$this->world->getGoal() == 'dungeons'")|p.Literal("in_array(config('game-mode'), ['open'])")).setParseAction(
        p.replaceWith({
            'boolean': False
        }))
game_mode = game_mode_true | game_mode_false
in_array_expr = G(
    s('in_array') + '(' + php_expr('member') - ',' - '[' -
    DelimitedList(item_ref | enum_string)('elements') - ']' - ')')

# Item classification: Group items into sets in source code and do class lookup.
item_is_a = (('is_a($item, Item\\' - identifier - '::class)') |
             ('$item instanceof Item\\' - identifier)).setName('item_is_a')
item_is_not = G('$item !=' - item_ref('item')).setName(
    'negative item assertion')
item_is = G('$item ==' - G(item_ref)('items')).setName('item assertion')
# End of ungrouped structures.

# Configuration options. The code generator will support the options and turn
# the PHP option queries into compile time constants.
world_config = G(
    opt('$this->world->') + 'config(' - p.quotedString('option') -
    opt(',' - (boolean | integer)('default')) - ')')

# Reduce to true/false with generation-time config options.
sword_left_to_place = p.Literal(
    '($this->world->getCurrentlyFillingItems()->hasSword())')

# Top-level expression alternates. These should be in groups so they get
# substructures.
item_in_locations = G('$locations->itemInLocations(Item::get(' - p.quotedString(
    'item') - ')' - ',' - '[' - DelimitedList(p.quotedString)(
        'allowable_locations') - ']' - ')').setName('item_in_location')

# Lookup into item count array. The major hack here is to replace a common
# sub-expression for a config option for dark room navigation with 1 (yes, one
# lamp is always required).
num_required_lamps = s(
    "$this->world->config('item.require.Lamp', 1)").setParseAction(
        p.replaceWith(1))
has_item = G(
    p.oneOf('$items $this') + '->has(' + p.quotedString('item') -
    opt(',' + (num_required_lamps | integer | php_var)('count')) - ')').setName(
        'has_item')
# Lookup into placement array.
location_has = (
    G('$locations[' + p.quotedString('location') + ']->hasItem(Item::get(' +
      G(p.quotedString)('items') + '))') |
    G('in_array($locations[' - p.quotedString('location') - ']->getItem(), '))

can_access = G(location_def('location') + '->canAccess(' + (
  p.Literal('$items').setParseAction(p.replaceWith('all_items')) |
  p.Literal('$this->world->collectItems()').setParseAction(p.replaceWith('uncle_item_only'))
  )('with_what') + ')').setName(
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
boss_can_beat = p.Literal('$this->boss->canBeat($items, $locations)'
                         ).setParseAction(lambda pr: current_region[0])

php_block = p.Forward().setName('block')
php_callable_expression = G(
    can_enter_or_complete('call_to_region_method') | (p.oneOf([
        'function($locations, $items)',
        'function($item, $locations, $items)',
        'function($item, $items)',
    ]) - G(php_block)('body'))).setName('callable')

php_atom = G(
    has_item('has_item') | location_has('location_has_item') |
    region_can_enter('access_to_region') | can_access('access_to_location') |
    method_call('call_builtin') | item_in_locations('item_in_locations') |
    game_mode('game_mode') | in_array_expr('in_array') |
    item_is_a('item_is_a') | item_is('item_is') | item_is_not('item_is_not') |
    world_config('config') | sword_left_to_place('sword_left_to_place') |
    boss_can_beat('boss') | boolean('boolean') | integer('integer') |
    php_callable_expression('callable') | php_var('var') |
    enum_string('enum')).setName('atom')

php_expr <<= infix_notation.infixNotation(
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
if_stmt = G('if' - s('(') - php_expr('condition') - ')' - G(php_block)
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
boss_entry = G('new static(' - p.quotedString('boss_name') - ',' -
               php_callable_expression('function') - '),')


def GetItemCollectionMethods():
  """Extract parse trees for the general purpose methods from ItemCollection."""
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
    sys.exit()
  for result in method_results:
    n = result[0].asDict()
    ret[n['name']] = n
  return ret


def GetBossRequirements():
  """Extract boss requirements."""

  item_collection = open('Boss.php').read()
  ret = {}
  try:
    boss_entries = boss_entry.searchString(item_collection)
  except (p.ParseException, p.ParseSyntaxException) as e:
    print('Bad Boss')
    print(e.lineno)
    print(e.line)
    print(' ' * (e.col - 1) + '^')
    print(e)
    sys.exit()
  for result in boss_entries:
    n = result[0].asDict()
    ret[n['boss_name']] = n

  return ret


def Smoosh(string):
  string = re.sub(r'[^a-z0-9]', '', string, flags=re.I)
  string = re.sub(r'([a-z])Of([A-Z])', r'\1of\2', string)
  return string


methods = GetItemCollectionMethods()
methods['bottleCount'] = {
    'body': [{
        'return': {
            'integer': '(bottle_count t)'
        }
    }]
}
bosses = GetBossRequirements()

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

boss_name_mapping = {
    'EasternPalace': 'Armos Knights',
    'DesertPalace': 'Lanmolas',
    'TowerofHera': 'Moldorm',
    'CastleTower': 'Agahnim',
    'PalaceofDarkness': 'Helmasaur King',
    'SwampPalace': 'Arrghus',
    'SkullWoods': 'Mothula',
    'ThievesTown': 'Blind',
    'IcePalace': 'Kholdstare',
    'MiseryMire': 'Vitreous',
    'TurtleRock': 'Trinexx',
    'GanonsTower': 'Agahnim2',
}


def ExpandToSMTLIB(d):
  name, value = next(iter(d.items()))
  # General constructs.
  if name == 'return':
    # SMTLIB is value-oriented with no statements. All statements we're
    # converting are either `if`s or `return`s, and the `if`s are just
    # conditional `return`s. These translate to `ite` expressions in
    # SMTLIB. Leading `if`s will emit a partial `ite` expression with condition
    # and the true case, and leave the false case and closing paren off.
    return '{}'.format(ExpandToSMTLIB(value))
  elif name == 'if':
    return '(ite {condition} {body} '.format(
        condition=ExpandToSMTLIB(value['condition']),
        body='\n'.join(ExpandToSMTLIB(stmt) for stmt in value['body']))
  elif name in ('body',):
    expression = '\n'.join(ExpandToSMTLIB(statement) for statement in value)
    expression += ')' * (expression.count('(') - expression.count(')'))
    return expression
  elif name in ('&&', '||', '*', '+'):
    operator = {'&&': 'and', '||': 'or'}.get(name, name)
    terms = ' '.join(ExpandToSMTLIB(expr) for expr in value)
    return f'({operator} {terms})'
  elif name == '!':
    return '(not {})'.format(ExpandToSMTLIB(value))
  elif name in ('<', '>=', '=='):
    if name == '==':
      name = '='
    return '({operator} {left} {right})'.format(
        left=ExpandToSMTLIB(value[0]), operator=name, right=ExpandToSMTLIB(value[1]))
  elif name == 'ternary':
    return '(ite {condition} {true} {false})'.format(
        condition=ExpandToSMTLIB(value['if']),
        true=ExpandToSMTLIB(value['then']),
        false=ExpandToSMTLIB(value['else']))
  elif name == 'boolean':
    return 'true' if value else 'false'
  # PHP constructs.
  elif name == 'var':
    # The var reference is returned as a formattable string piece.
    return '{%s}' % value['symbol']
  elif name == 'integer':
    return str(value)
  elif name == 'enum':
    return MakeConstant(value)
  elif name == 'in_array':
    if value['member'] == {'var': {'symbol': 'item'}}:
      return '(or ' + ' '.join(
          f'(= item {element})' for element in value['elements']) + ')'
    else:
      return '(or ' + ' '.join(
          '(= {} {})'.format(ExpandToSMTLIB(value['member']), element)
          for element in value['elements']) + ')'
    import pdb
    pdb.set_trace()
  # ALTTP specifics.
  elif name == 'config':
    return 'CONFIG_OPTION_' + MakeConstant(value['option'])
  elif name == 'call_to_region_method':
    return f'({value["method_name"]} {value["region"]} t)'
  elif name == 'call_builtin':
    method_param = methods[value['method_name']].get('parameter')
    method_body = ExpandToSMTLIB(methods[value['method_name']]['body'][0]['return'])
    if method_param:
      parameter_name = method_param['symbol']
      actual_parameter = value.get('actual_parameter',
                                   methods[value['method_name']].get('default'))
      method_body = method_body.format(**{parameter_name: actual_parameter})
    return method_body
  elif name == 'boss':
    return ExpandToSMTLIB(
        bosses[boss_name_mapping[value]]['function']['body'][0]['return'])
  elif name == 'sword_left_to_place':
    return '(> (num_unplaced ProgressiveSword) 0)'
  elif name == 'has_item':
    n = value.get('count', 1)
    if isinstance(n, dict):
      n = '{%s}' % n['symbol']
    return '(is_num_reachable {n} (as {item} Item) (- t 1))'.format(
        item=value['item'], n=n)
  elif name == 'location_has_item':
    return '(or ' + ' '.join(
        '(at (as {location} Location) (as {item} Item))'.format(
            location=Smoosh(value['location']), item=Smoosh(item))
        for item in value['items']) + ')'
  elif name == 'item_in_locations':
    return '(or ' + ' '.join(
        '(at {location} {item})'.format(
            location=Smoosh(location), item=Smoosh(value['item']))
        for location in value['allowable_locations']) + ')'
  elif name == 'access_to_region':
    if value['region'] == '$this':
      return 'true'
    else:
      return '(can_enter {} (- t 1))'.format(region_name_mapping.get(
          Smoosh(value['region']), Smoosh(value['region'])))
  elif name == 'access_to_location':
    if value['with_what'] == 'all_items':
      return '(access (as {} Location) (- t 1))'.format(Smoosh(value['location']))
    elif value['with_what'] == 'uncle_item_only':
      # Special casing standard uncle weapons.
      other_terms = '(or ' + ' '.join(
          f'(at LinksUncle {weapon})'
          for weapon in [
              'CaneOfByrna',
              'CaneOfSomaria',
              'TenBombs',
              'Bow',
              'Hammer',
              'FireRod',
          ]) + ')'
      sword_term = ('(and (at LinksUncle ProgressiveSword) '
                    '(= (num_unplaced ProgressiveSword) 0))')
      return f'(or {sword_term} {other_terms})'
    else:
      raise Exception('Missed a case.')
  elif name == 'config':
    default = value['default']
    if isinstance(default, bool):
      return ExpandToSMTLIB({'boolean': default})
    elif isinstance(default, int):
      return str(default)
    else:
      assert False
  elif name == 'game_mode':
    return ExpandToSMTLIB(value)
  elif name == 'item_is':
    return '(or {})'.format(' '.join(
        f'(= item {option})' for option in value['items']))
  elif name == 'item_is_not':
    return f'(not (= item {value["item"][0]}))'
  elif name == 'item_is_a':
    return 'false'  # Massive shortcut!
  else:
    raise Error('Unhandled case: {}. Next level: {}'.format(name, value))
