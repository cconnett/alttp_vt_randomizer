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


def GetMentionedThings(location_pattern, things, text):
  match = re.search(location_pattern, text)
  if match:
    matched_text = match.group(2)
    for item in things:
      if item in matched_text:
        yield item


def WalkSources():
  for root, _, files in os.walk('Region/'):
    for filename in files:
      path = os.path.join(root, filename)
      if path.endswith('.php'):
        yield (open(path).read(), path[len('Region/'):-len('.php')])


locations = [line.strip() for line in open('locations.txt')]
items = [line.strip() for line in open('items.txt')]


def BuildIndex():
  """Parse some PHP."""
  errors = []
  for location in locations:
    for source, region in WalkSources():
      location = location.strip()
      pattern = re.compile(r'\["({})"\]->setRequirements\((.*)'.format(
          re.escape(location)), re.DOTALL | re.MULTILINE)

      source2 = re.search(r'initNoMajorGlitches.*?/\*\*', source,
                          re.DOTALL | re.MULTILINE)

      if not source2:
        continue
      source3 = source2.group(0)
      match = pattern.search(source3)
      if match:
        found_location = match.group(1)
        body = match.group(2)
        c_location = php_grammar.Smoosh(found_location)
        c_region = php_grammar.Smoosh(region)
        print('case Locations::{}:'.format(c_location))
        print('if (!this->can_enter(Locations::Regions::{})) {{ '
              'return false; }}'.format(c_region))
        e = php_grammar.php_lambda('root').parseString(body)
        # pprint.pprint(ToTupleList(e['root']))
        print(' '.join(php_grammar.ExpandToC(ToTupleList(e[0]))))
        break


header, footer = re.split(
    r'^\s*// Generated code goes here\.$',
    open('world.cc').read(),
    maxsplit=1,
    flags=re.DOTALL | re.MULTILINE)
print(header)
BuildIndex()
print(footer)
