import array
import ctypes
import os
import struct

import mappings

item_location_pairs = [
  ('Bombos', 'LinksUncle')
  ]

structure = struct.Struct('<I')

candidates = set()
for (item, location) in item_location_pairs:
  print(item, location)
  if isinstance(item, str):
    item = mappings.items[item]
  if isinstance(location, str):
    location = mappings.locations[location]
  print(item, location)
  path = 'bigresults/item{:03d}-location{:03d}'.format(item, location)
  matches = set()
  for filename in os.listdir(path):
    #buf = ctypes.create_string_buffer(open(os.path.join(path, filename), 'rb').read())
    a = array.array('I')
    try:
      a.fromfile(open(os.path.join(path, filename), 'rb'), 8 * 1024 * 1024)
    except EOFError:
      pass
    print(len(a))
    matches |= set(a)
  print(len(matches))
  if not candidates:
    candidates = matches
  else:
    candidates &= matches
  if len(candidates) == 1:
    print('Match found: {}'.format(list(candidates)[0]))
    break
else:
  print('{} seeds remaining.'.format(len(candidates)))
  if len(candidates) <= 10:
    print(candidates)
