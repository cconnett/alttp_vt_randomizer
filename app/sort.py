import os
import sys

import numpy

root = sys.argv[1]

for item in range(1, 157):
  for location in range(1, 235):
    if item < 64  or item == 64 and location < 84:
      continue
    print(f'Building {item} x {location}.')
    acc = numpy.array([], dtype='uint32')
    for batch in range(1, 8):
      rel = f'{root}/batch{batch}/item{item:03d}-location{location:03d}/'
      if acc.size > 200000000:
        print('Skip.')
        break
      for name in os.listdir(rel):
        fname = os.path.join(rel, name)
        acc = numpy.append(acc, numpy.fromfile(fname, dtype='uint32'))
        if acc.size > 200000000:
          break
    acc.sort()
    out = f'/mnt/hgfs/G/bigresults/i{item:03d}-l{location:03d}'
    acc.tofile(out)

