"""Script for generating the seeds.db schema."""

from __future__ import print_function

tmpl = ('CREATE TABLE IF NOT EXISTS '
        'seeds_with_item{item:03d}_at_location{location:03d} (seed int);')
f = open('seeds.schema', 'w')
for item in range(1, 157):
  for location in range(1, 235):
    print(tmpl.format(item=item, location=location), file=f)
