"""Build an index for ALTTP project."""

from __future__ import print_function

import os
import re

import php_grammar
from php_grammar import ToTupleList


class Error(Exception):
  pass


def DisplayPR(pr, level=0):
  if isinstance(pr, str):
    return
  print(('  ' * level) + 'Name: ' + str(pr.getName()))
  print(('  ' * level) + 'Keys: ' + str(list(pr.keys())))
  print(('  ' * level) + 'Len : ' + str(len(pr)))
  for key in pr.keys():
    if isinstance(pr[key], str):
      print(('  ' * (level + 1)) + '{}:{}'.format(key, pr[key]))
    else:
      DisplayPR(pr[key], level=level + 1)


def WalkSources():
  for root, _, files in os.walk('Region/'):
    for filename in files:
      path = os.path.join(root, filename)
      if path.endswith('.php'):
        yield (open(path).read(), path[len('Region/'):-len('.php')])


def BuildIndex():
  """Parse some PHP."""
  # By location
  can_reach = {
      # This location has no fill rules and no requirements and thus does not
      # appear in the source file for the region. We'll need to manually give it
      # an entry.
      'SkullWoodsMapChest': {
          'return': {
              'access_to_region': {
                  'region': 'SkullWoods'
              }
          }
      }
  }
  fill_rules = {}
  # By region
  can_enter = {}
  can_complete = {}
  for source, region in WalkSources():
    region = php_grammar.Smoosh(region)
    for e in php_grammar.init_no_major_glitches('root').searchString(source):
      s = ToTupleList(e.root.definitions)
      if not isinstance(s, list):
        s = [s]
      for entry in s:
        if entry.get('region_method') == ['can_enter']:
          can_enter[region] = entry['rhs']
        elif entry.get('region_method') == ['can_complete']:
          can_complete[region] = entry['rhs']
        elif entry.get('location'):
          location = php_grammar.Smoosh(entry['location'])
          if location == 'Prize':
            location = region + location
          r = entry['riders']
          if isinstance(r, tuple):
            r = {r[0]: r[1]}
          can_reach[location] = r.get('requirements')
          if can_reach[location]:
            if 'region_method_call' in can_reach[location]:
              method_name = can_reach[location]['region_method_call']
              can_reach[location]['region_method_call'] = {
                  'method_name': method_name,
                  'region': region
              }
            elif 'body' in can_reach[location]:
              can_reach[location]['body'].insert(
                  0, {
                      'if': {
                          'cond': {
                              'not': {
                                  'access_to_region': {
                                      'region': region
                                  }
                              }
                          },
                          'body': [{
                              'return': {
                                  'boolean': 'false'
                              }
                          }]
                      }
                  })
          else:
            can_reach[location] = {
                'body': [{
                    'return': {
                        'access_to_region': {
                            'region': region
                        }
                    }
                }]
            }
          if 'fill_rules' in r:
            fill_rules[location] = r['fill_rules']

  return can_reach, can_enter, can_complete, fill_rules


can_reach, can_enter, can_complete, fill_rules = BuildIndex()


def CodeFor(methods, namespace='Location::', injection=None):
  for place in sorted(methods.keys()):
    yield 'case {namespace}{place}:'.format(namespace=namespace, place=place)
    if injection:
      yield injection.format(place=place)
    yield ' '.join(php_grammar.ExpandToC(methods[place]))


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
print(code)
