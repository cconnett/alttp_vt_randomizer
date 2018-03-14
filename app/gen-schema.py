"""Script for generating the seeds.db schema."""

from __future__ import print_function

tmpl = ('CREATE TABLE IF NOT EXISTS '
        'item{:03d} '
        '(seed integer primary key);')
f = open('seeds.schema', 'w')
for item in range(1, 157):
  print(tmpl.format(item), file=f)
