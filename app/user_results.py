import array
import ctypes
import os
import subprocess

import mappings

location_item_pairs = [
  ]

candidates = set()
for (location, item) in location_item_pairs:
  print(location, item)
  if isinstance(item, str):
    item = mappings.items[item]
  if isinstance(location, str):
    location = mappings.locations[location]
  print(f'{mappings.locations[location]} has {mappings.items[item]}')
  matches = set()
  for batch in range(1, 7):
    path = f'bigresults/batch{batch}/item{item:03d}-location{location:03d}'
    for filename in os.listdir(path):
      a = array.array('I')
      try:
        a.fromfile(open(os.path.join(path, filename), 'rb'), 8 * 1024 * 1024)
      except EOFError:
        pass
      matches |= set(a)
  if not candidates:
    candidates = matches
  else:
    candidates &= matches
  if len(candidates) == 1:
    winner = list(candidates)[0]
    subprocess.call('./bazel-bin/makeseed {winner}'.format(winner=winner), shell=True)
    print('Match found: {winner}'.format(winner=winner))
    print('bazel-bin/makeseed {winner}'.format(winner=winner))
    break
  else:
    print('{} seeds remaining.'.format(len(candidates)))
    if len(candidates) <= 10:
      print(candidates)
