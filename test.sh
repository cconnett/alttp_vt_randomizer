function runtest {
  seed="$1"
  shift
  php artisan -v alttp:randomize --skip-md5 /dev/null /tmp --no-rom --spoiler --seed=${seed} --mode=open \
    | fgrep " := " \
    | egrep -v "(Waterfall|Pyramid)Bottle" \
    | egrep -v "= Defeat|PyramidFairy(Bow|Sword)" \
    | sed 's/:=/=/' \
    | sort \
    > /tmp/php
  app/bazel-bin/makeseed "${seed}" \
    | fgrep " = " \
    | fgrep -v "Prize =" \
    | fgrep -v "SkullWoodsPinballRoom =" \
    | egrep -v "= Defeat|PyramidFairy(Bow|Sword)" \
    | sort \
    > /tmp/c
  diff -u /tmp/php /tmp/c
}

SEEDS=$(python -c 'import random; print(" ".join(str(random.randint(1, 10**9)) for _ in range(100)))')

# These seeds have a tricky issue. Adding access to a BigKeyD5 can increase the
# requirements for another chest.
SEEDS="992839880 365791332 635470466"

for i in $SEEDS; do
  echo $i
  runtest $i || exit $?
done
