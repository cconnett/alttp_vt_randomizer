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


def AccessToRegion(region):
  return {'return': {'access_to_region': {'region': region}}}


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
      can_reach[location] = {'body': [AccessToRegion(region)]}

    # Search for the initNoMajorGlitches function and read its code.
    try:
      init_nmg_block = php_grammar.init_no_major_glitches.searchString(source)[
          0][0]
    except p.ParseException as e:
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
        can_reach[location] = AccessToRegion(region)

        # Store each predicate that is explicitly set on the location.
        for predicate in value['predicates']:
          {
              'setRequirements': can_reach,
              'setFillRules': fill_rules,
              'setAlwaysAllow': always_allow,
          }[predicate['slot']][location] = ApplyAccessToRegion(
              code=predicate['function'], region=region)
      elif name == 'region_method_definition':
        method_suite = {
            'can_enter': can_enter,
            'can_complete': can_complete,
        }[value['region_method']['method_name']]
        method_suite[region] = value['rhs']
      else:
        raise Error('Unhandled case: {}. Next level: {}'.format(name, value))

  return can_reach, can_enter, can_complete, fill_rules, always_allow


def ApplyAccessToRegion(code, region):
  """Returns a modified version of code that checks for access to region."""
  code = code.copy()
  if 'body' in code:
    # The code is a lambda with a code body we can modify.
    if len(code['body']) == 1 and 'return' in code['body'][0]:

      # The code is a single return statement with an expression. Replace the
      # return value with an `and` of AccessToRegion and the original value.
      code['body'][0]['return'] = {
          'and': [AccessToRegion(region)['return'], code['body'][0]['return']]
      }
    else:
      # It's not a simple return statement. Just insert a new statement at the
      # beginning: `if (!AccessToRegion) { return false; }`.
      code['body'].insert(
          0, {
              'if': {
                  'condition': {
                      'not': AccessToRegion(region)['return']
                  },
                  'body': [{
                      'return': {
                          'boolean': False
                      }
                  }],
              }
          })
  elif 'call_to_region_method' in code:
    code['call_to_region_method']['region'] = region
  else:
    raise Error('Unhandled case: {}.'.format(code))
  return code


def CodeFor(methods, namespace='Location::', injection=None):
  for place in sorted(methods.keys()):
    yield 'case {namespace}{place}:'.format(namespace=namespace, place=place)
    if injection:
      yield injection.format(place=place)
    yield php_grammar.ExpandToC(methods[place])
    # The previous line should emit a one or more statements that always return,
    # but in case it doesn't, emit a safety `break`.
    yield 'break;'


def main():
  can_reach, can_enter, can_complete, fill_rules, always_allow = BuildIndex()

  code = open('world_template.cc').read()
  code = re.sub(
      r'^.*// <SUB:can_reach>.*$',
      ' '.join(CodeFor(can_reach)),
      code,
      flags=re.MULTILINE)
  code = re.sub(
      r'^.*// <SUB:can_enter>.*$',
      ' '.join(CodeFor(can_enter, namespace='Region::')),
      code,
      flags=re.MULTILINE)
  code = re.sub(
      r'^.*// <SUB:can_complete>.*$',
      ' '.join(CodeFor(can_complete, namespace='Region::')),
      code,
      flags=re.MULTILINE)
  code = re.sub(
      r'^.*// <SUB:can_fill>.*$',
      ' '.join(CodeFor(fill_rules)),
      code,
      flags=re.MULTILINE)
  code = re.sub(
      r'^.*// <SUB:always_allow>.*$',
      ' '.join(CodeFor(always_allow)),
      code,
      flags=re.MULTILINE)
  print(code)


if __name__ == '__main__':
  try:
    main()
  except Exception as e:
    pdb.post_mortem()
