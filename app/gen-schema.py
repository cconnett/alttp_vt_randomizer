"""Script for generating the seeds.db schema."""

from __future__ import print_function

tmpl = ('CREATE TABLE IF NOT EXISTS '
        'location{location:03d} '
        '(seed integer primary key);')
f = open('seeds.schema', 'w')
for location in range(1, 235):
  print(tmpl.format(location=location), file=f)
