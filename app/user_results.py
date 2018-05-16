import bisect
import functools
import os
import subprocess
import sys

import numpy

import mappings

location_item_pairs = [
  ('LinksHouse', ''),
  ('LinksUncle', ''),
  ('SecretPassage', ''),
  ('HyruleCastleMapChest', ''),
  ('HyruleCastleBoomerangChest', ''),
  ('HyruleCastleZeldasCell', ''),
  ('SewersDarkCross', ''),
  ('SewersSecretRoomLeft', ''),
  ('SewersSecretRoomMiddle', ''),
  ('SewersSecretRoomRight', ''),
  ('Sanctuary', ''),
  ('LumberjackTree', ''),
  ('Mushroom', ''),
  ('LostWoodsHideout', ''),
  ('', ''),
  ]

@functools.total_ordering
class SeedIterator(object):
  def __init__(self, item, location, is_mmap=True):
    self.item = item
    self.location = location
    self.filename = f'/mnt/hgfs/G/bigresults/i{item:03d}-l{location:03d}'
    self.stream = (numpy.memmap if is_mmap else numpy.fromfile)(
      self.filename, dtype='uint32')
    self.offset = 0
  def __lt__(self, other):
    # N.B.: Inverting ordering to get a max heap
    return self.current > other.current
  def __eq__(self, other):
    return self.current == other.current

  @property
  def current(self):
    try:
      return self.stream[self.offset]
    except IndexError:
      return None

  def advance_to(self, x):
    self.offset = bisect.bisect_left(self.stream, x, lo=self.offset)
    return self.current

  def advance_any(self):
    self.offset += 1
    return self.current

streams = []
for (location, item) in location_item_pairs:
  if not item or not location:
    continue
  if isinstance(item, str):
    item = mappings.items[item]
  if isinstance(location, str):
    location = mappings.locations[location]
  streams.append(SeedIterator(item, location))

if len(streams) < 2:
  print('Need at least 2 observations.')
  sys.exit()

streams.sort(key=lambda stream: os.stat(stream.filename).st_size)
max_location = max(len(mappings.locations[stream.location])
                     for stream in streams)
max_item = max(len(mappings.items[stream.item]) for stream in streams)
for stream in streams:
  n = os.stat(stream.filename).st_size // 4
  print(
    f'{mappings.locations[stream.location]:{max_location}} has '
    f'{mappings.items[stream.item]:{max_item}} in {n:9d} seeds.')
a = SeedIterator(streams[0].item, streams[0].location, is_mmap=False)
b = SeedIterator(streams[1].item, streams[1].location, is_mmap=False)
streams = streams[2:]
matches = sorted(set(a.stream) & set(b.stream))

for stream in streams:
  print('{} seeds remaining.'.format(len(matches)))
  matches = [seed for seed in matches if stream.advance_to(seed) == seed]
  if not matches:
    print('Went bust.')
    break
  elif len(matches) == 1:
    winner = list(matches)[0]
    print('Match found: {winner}'.format(winner=winner))
    print('bazel-bin/makeseed {winner} | grep BigKeyA2'.format(winner=winner))
    break
else:
  print('{} seeds remaining.'.format(len(matches)))
  if len(matches) <= 10:
    print(matches)
