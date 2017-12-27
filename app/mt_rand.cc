#include "mt_rand.h"
#include <iomanip>
#include <iostream>
#include "PHP_mt19937.h"

PHP_mt19937 generator;

int scale(uint n, uint min, uint max) {
  return min + (uint)((max - min + 1.0) * (((double)n) / (uint)0x7fffffff));
}

unsigned int mt_rand(uint min, uint max) {
  int raw = generator.random();
  int ret = scale(raw, min, max);
#ifndef NDEBUG
  std::cout << "mt_rand(" << min << ", " << max << ") = " << ret << std::endl;
#endif
  return ret;
}
void mt_srand(int seed) { generator.seed(seed); }
