"""Build an index for ALTTP project."""

from __future__ import print_function
import os
import re

import php_grammar


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
        yield (open(path).read(), path[:-len('.php')])


locations = [line.strip() for line in open('locations.txt')]
items = [line.strip() for line in open('items.txt')]


def BuildIndex():
  """Parse some PHP."""
  errors = []
  for location in locations:
    for source, _ in WalkSources():
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
        try:
          print(found_location)
          php_grammar.php_expr.parseString(body)
          break
        except php_grammar.p.ParseException as pe:
          errors.append((found_location, body, pe))

  _, columns = os.popen('stty size', 'r').read().split()
  for e in errors:
    print('{:<20}\t{}'.format(e[0][:20],
                              re.sub(r'\s+', ' ', e[1][:int(columns) - 20])))
  if errors:
    location, body, pe = errors[0]
    print(location)
    lines = body.splitlines()
    print('\n'.join(lines[:pe.lineno]))
    print(' ' * pe.col + '^')
    print(pe)
    print('\n'.join(lines[pe.lineno:pe.lineno + 3]))


BuildIndex()
