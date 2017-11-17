"""Partial grammar for PHP."""

import pyparsing as p
s = p.Suppress
p.ParserElement.enablePackrat()
p.quotedString.addParseAction(p.removeQuotes)

boolean = p.oneOf('true false').setName('boolean')
item_in_locations = (
    s('$locations->itemInLocations(Item::get(') + p.quotedString('item') +
    s('), [') + p.Group(p.quotedString + p.ZeroOrMore(',' + p.quotedString) +
                        p.Optional(','))('allowable_locations') +
    s('])')).setName('item_in_location')

# Lookup into item count array.
has_item = p.Group(
    s('$items->has(') + p.quotedString('item') +
    p.Optional(s(',') + p.Word(p.nums)('count')) + s(')')).setName('has_item')
# Lookup into placement array.
locations_has = p.Group(
    s('$locations[') + p.quotedString('location') + s(']->hasItem(Item::get(') +
    p.quotedString('item') + s('))')).setName('location_has')

# Generated global functions (or macros).
region_can_enter = (
    s('$this->world->getRegion(') + p.quotedString('region') +
    s(')->canEnter($locations, $items)')).setName('region_can_enter')
method_call = (
    s('$items->') + p.Word(p.alphas) + s('()')).setName('method_call')
can_complete = p.Literal('$this->can_complete').setName(
    'can_complete').setParseAction(p.replaceWith('can_complete'))

# Reduced to true/false under assumption of open mode.
game_mode = (p.Literal("config('game-mode') == 'swordless'").setParseAction(
    p.replaceWith('false')) |
             p.Literal("in_array(config('game-mode'), ['open', 'swordless'])")
             .setParseAction(p.replaceWith('true')))
# Reduce to true/false with generation-time config options.
world_config = p.Group(
    s('$this->world->config(') + p.quotedString('option') + s(',') +
    boolean('value') + s(')')).setName('config option')

# Item classification: Group items into sets in source code and do class lookup.
is_a = (
    s('is_a($item, Item\\') + p.Word(p.alphas) + s('::class)')).setName('is_a')

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

# These can be nearly copied over nearly in tact.
return_stmt = (s('return') + php_expr + s(';')).setName('return statement')
if_stmt = p.Group(
    s('if (') + php_expr('condition') + s(')') + php_block('action')).setName(
        'if_statment')
php_stmt = (return_stmt('return') | if_stmt('if')).setName('statement')
php_block <<= p.Group(s('{') + p.OneOrMore(php_stmt) + s('}'))
