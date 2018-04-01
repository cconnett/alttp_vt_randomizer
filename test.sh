PATH=/usr/local/silly/bin:$PATH

function runtest {
  seed="$1"
  shift
  php artisan -v alttp:randomize --skip-md5 /dev/null /tmp --no-rom --spoiler --seed=${seed} --mode=open \
    | fgrep -v "mt_rand" \
    | fgrep " := " \
    | egrep -v "(Waterfall|Pyramid)Bottle" \
    | egrep -v "= Defeat|PyramidFairy(Bow|Sword)" \
    | sed 's/:=/=/' \
    | sort \
    > /tmp/php
  app/bazel-bin/makeseed "${seed}" \
    | fgrep " = " \
    | fgrep -v "mt_rand" \
    | fgrep -v "SkullWoodsPinballRoom =" \
    | egrep -v "Defeat|Rescue|PyramidFairy(Bow|Sword)" \
    | egrep -v "(TurtleRock|MiseryMire)Medallion" \
    | sort \
    > /tmp/c
  diff -u /tmp/php /tmp/c
}

# Smoke test on the first ten seeds.
for i in {1..10}; do
  echo $i
  runtest $i || exit $?
done

# These seeds have a tricky issue. Adding access to a BigKeyD5 can increase the
# requirements for another chest.
SEEDS="992839880 365791332 635470466 265824031"
for i in $SEEDS; do
  echo $i
  runtest $i || exit $?
done

# Finally, test 100 random seeds.
SEEDS=$(python -c 'import random; print(" ".join(str(random.randint(1, 10**9)) for _ in range(100)))')
for i in $SEEDS; do
  echo $i
  runtest $i || exit $?
done
