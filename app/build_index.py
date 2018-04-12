#!/usr/bin/python3.6
"""Build an index for ALTTP project."""

from __future__ import print_function

import os
import pdb
import re

import php_grammar
import pyparsing as p


class Error(Exception):
  pass


def WalkSources():
  for root, _, files in os.walk('Region/'):
    for filename in files:
      path = os.path.join(root, filename)
      if path.endswith('.php'):
        yield (open(path).read(), path[len('Region/'):-len('.php')])


def BuildIndex():
  """Parse some PHP."""
  # By location
  can_reach = {}
  fill_rules = {}
  always_allow = {}
  # By region
  can_enter = {}
  can_complete = {}

  for source, region in WalkSources():
    region = php_grammar.Smoosh(region)
    if region in ('Fountains', 'Medallions'):
      continue
    # Find all locations mentioned anywhere in the file and default their
    # can_reach requirements to can_enter(region).
    locations = set(
        re.findall(r'''\$this->locations\[['"](.*?)['"]\]''', source))
    for location in locations:
      location = php_grammar.Smoosh(location)
      if location in ('Ganon', 'Agahnim', 'Agahnim2'):
        continue
      can_reach[location] = {
          'body': [{
              'return': {
                  'access_to_region': {
                      'region': region
                  }
              }
          }]
      }

    php_grammar.current_region[0] = region
    # Search for the initNoMajorGlitches function and read its code.
    try:
      init_nmg_block = php_grammar.init_no_major_glitches.searchString(source)[
          0][0]
    except (p.ParseException, p.ParseSyntaxException) as e:
      print('Bad', region)
      print(e.lineno)
      print(e.line)
      print(' ' * (e.col - 1) + '^')
      print(e)
      import sys
      sys.exit()
    definition_list = init_nmg_block.asDict()['definitions']
    for entry in definition_list:
      assert len(entry) == 1
      name, value = list(entry.items())[0]
      if name == 'location_definition':
        # Massage the location name.
        location = php_grammar.Smoosh(value['location'])
        if location == 'Prize':
          location = region + location

        # Set the default can_reach value to be access to the region.
        can_reach[location] = {
            'return': {
                'access_to_region': {
                    'region': region
                }
            }
        }

        # Store each predicate that is explicitly set on the location.
        for predicate in value['predicates']:
          if predicate['slot'] == 'setRequirements':
            can_reach[location] = ApplyAccessToRegion(
                predicate['function'], region=region)
          elif predicate['slot'] == 'setFillRules':
            fill_rules[location] = predicate['function']
          elif predicate['slot'] == 'setAlwaysAllow':
            always_allow[location] = predicate['function']
          else:
            assert False
      elif name == 'region_method_definition':
        if value['region_method']['method_name'] == 'can_enter':
          can_enter[region] = value['rhs']
        elif value['region_method']['method_name'] == 'can_complete':
          can_complete[region] = ApplyAccessToRegion(
              value['rhs'], region=region)
      else:
        raise Error('Unhandled case: {}. Next level: {}'.format(name, value))

  return can_reach, can_enter, can_complete, fill_rules, always_allow


def ApplyAccessToRegion(code, region):
  """Returns a modified version of code that checks for access to region.

  This function does evil AST-based code rewriting that makes some assumptions.

  Args:
    code: Code to modify.
    region: The current region (checks for access to region are inserted).
  """
  code = code.copy()
  if 'body' in code:
    # The code is a lambda with a code body we can modify.
    if len(code['body']) == 1 and 'return' in code['body'][0]:
      # The code is a single return statement with an expression.
      returned_expr = code['body'][0]['return']

      if ('&&' in returned_expr and
          'access_to_region' in returned_expr['&&'][0] and
          returned_expr['&&'][0]['access_to_region']['region'] == '$this'):
        # The code starts with an access check for $this, which we need to
        # replace with the current region (which comes from the filename being
        # parsed, which only this module has).
        returned_expr['&&'][0]['access_to_region']['region'] = region
      else:
        # Prefix the returned expression with an `and` of access_to_region and
        # the original value.
        code['body'][0]['return'] = {
            '&&': [{
                'access_to_region': {
                    'region': region
                }
            }, returned_expr]
        }
    else:
      # It's not a simple return statement. Just insert a new statement at the
      # beginning: `if (!access_to_region) { return false; }`.
      code['body'].insert(
          0, {
              'if': {
                  'condition': {
                      '!': {
                          'access_to_region': {
                              'region': region
                          }
                      }
                  },
                  'body': [{
                      'return': {
                          'boolean': False
                      }
                  }],
              }
          })
  elif 'call_to_region_method' in code:
    # The code is a call to another method defined on the region, e.g.,
    # can_complete. In theory we should also add a similar check for access to
    # the region, but in practice all the can_complete definitions in the PHP
    # include that check in the code.

    # Since the current region isn't determined from the code itself (it comes
    # from the name of the file being read), we have to go out of our way to
    # attach the region to the parse result so the code generator knows what
    # region to use.
    code['call_to_region_method']['region'] = region
  else:
    raise Error('Unhandled case: {}.'.format(code))
  return code


def CodeFor(method_cases, canonical_form, cells):
  for place in sorted(method_cases.keys()):
    method_expr = php_grammar.ExpandToSMTLIB(method_cases[place])
    interpolated_case = canonical_form.format(place=place)
    yield f'\n(assert (forall ({cells}) (= {interpolated_case} {method_expr})))'


def main():
  can_reach, can_enter, can_complete, fill_rules, always_allow = BuildIndex()
  del can_reach['Zelda']
  can_reach['Mushroom'] = '(as Mushroom Location)'
  del can_reach['Mushroom']

  code = open('world_template.cc').read()
  code = re.sub(
      r'^.*// <SUB:can_reach>.*$',
      ' '.join(CodeFor(can_reach, '(access {place} t)', '(t Int)')),
      code,
      flags=re.MULTILINE)
  code = re.sub(
      r'^.*// <SUB:can_enter>.*$',
      ' '.join(CodeFor(can_enter, '(can_enter {place} t)', '(t Int)')),
      code,
      flags=re.MULTILINE)
  code = re.sub(
      r'^.*// <SUB:can_complete>.*$',
      ' '.join(CodeFor(can_complete, '(can_complete {place} t)', '(t Int)')),
      code,
      flags=re.MULTILINE)
  code = re.sub(
      r'^.*// <SUB:can_fill>.*$',
      ' '.join(CodeFor(fill_rules, '(can_fill {place} item)', '(item Item)')),
      code,
      flags=re.MULTILINE)
  code = re.sub(
      r'^.*// <SUB:always_allow>.*$',
      ' '.join(CodeFor(always_allow, '(always_allow {place} item t)',
                       '(item Item) (t Int)')),
      code,
      flags=re.MULTILINE)
  print(code)


if __name__ == '__main__':
  try:
    main()
  except Exception as e:
    pdb.post_mortem()
