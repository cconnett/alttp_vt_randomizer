#include "PHP_mt19937.h"

#include "mt_rand.h"

PHP_mt19937 generator;

int scale(uint n, uint min, uint max) {
  return min + (uint)((max - min + 1.0) * (((double)n) / (uint)0x7fffffff));
}

uint mt_rand(uint min, uint max) { return scale(generator.random(), min, max); }
void php_srand(int seed) { generator.seed(seed); }
