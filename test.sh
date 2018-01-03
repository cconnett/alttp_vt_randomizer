function runtest {
  seed="$1"
  shift
  php artisan -v alttp:randomize --skip-md5 /tmp/rom /tmp --no-rom --spoiler --seed=${seed} --mode=open > /dev/null
  sort "/tmp/alttp - VT_no-glitches-27_normal_open_none_${seed}.txt" \
    | egrep -v "(Waterfall|Pyramid)Bottle" \
    > /tmp/php

  app/bazel-bin/makeseed "${seed}" | \
    sort | \
    egrep -v "= Defeat|PyramidFairy(Bow|Sword)" \
    > /tmp/c
  diff -u /tmp/php /tmp/c
}

SEEDS=$(python -c 'import random; print " ".join(str(random.randint(1, 10**9)) for _ in range(100))')
for i in $SEEDS; do
  echo $i
  runtest $i
done
