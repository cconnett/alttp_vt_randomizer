function runtest {
  seed="$1"
  shift
  php artisan -v alttp:randomize --skip-md5 /tmp/rom /tmp --no-rom --spoiler --seed=${seed} --mode=open > /dev/null
  sort "/tmp/alttp - VT_no-glitches-27_normal_open_none_${seed}.txt" \
    | egrep -v "(Waterfall|Pyramid)Bottle" \
    > /tmp/php

  echo "${seed}" | app/bazel-bin/makeseed | \
    sort | \
    egrep -v "= Defeat|PyramidFairy(Bow|Sword)" \
    > /tmp/c
  diff -u /tmp/php /tmp/c
}

runtest 2
