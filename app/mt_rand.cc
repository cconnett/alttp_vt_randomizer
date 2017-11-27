#include "PHP_mt19937.h"

#include "mt_rand.h"

PHP_mt19937 generator;

int scale(int n, int min, int max) {
  return (int)(min + (max - min + 1.0) * (((double)n) / (1 << 31)));
}

void php_srand(int seed) { generator.seed(seed); }

uint mt_rand(uint min, uint max) { return scale(generator.random(), min, max); }
