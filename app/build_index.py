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
  can_reach = {}
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
        if entry.get('region_method', None) == 'can_enter':
          can_enter[region] = entry['rhs']
        elif entry.get('region_method', None) == 'can_complete':
          can_complete[region] = entry['rhs']
        elif entry.get('location', None):
          location = php_grammar.Smoosh(entry['location'])
          if location == 'Prize':
            location = region + location
          r = entry['riders']
          if isinstance(r, tuple):
            r = {r[0]: r[1]}
          can_reach[location] = r.get('requirements')
          if can_reach[location]:
            if can_reach[location].get('region_method_call', None):
              method_name = can_reach[location]['region_method_call']
              can_reach[location]['region_method_call'] = {
                  'method_name': method_name,
                  'region': region
              }
          fill_rules[location] = r.get('fill_rules')

  return can_reach, can_enter, can_complete, fill_rules


can_reach, can_enter, can_complete, fill_rules = BuildIndex()


def CodeFor(methods, namespace='Location::'):
  for place in sorted(methods.keys()):
    yield 'case {namespace}{place}:'.format(namespace=namespace, place=place)
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
# code = re.sub(
#     r'^.*// <SUB:can_complete>.*$',
#     ' '.join(CodeFor(can_complete, namespace='Region::')),
#     code,
#     flags=re.MULTILINE)
# code = re.sub(
#     r'^.*// <SUB:fill_rules>.*$',
#     ' '.join(CodeFor(fill_rules)),
#     code,
#     flags=re.MULTILINE)
print(code)
