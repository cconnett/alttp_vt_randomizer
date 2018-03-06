#include <iomanip>
#include <iostream>

#include "mt_rand.h"
#include "prngs/PHP_mt19937.h"

int scale(uint n, uint min, uint max) {
  return min + (uint)((max - min + 1.0) * (((double)n) / (uint)0x7fffffff));
}

unsigned int mt_rand::rand(uint min, uint max) {
  int raw = ((PHP_mt19937 *)generator)->random();
  int ret = scale(raw, min, max);
#ifdef RAND_SYNC
  std::cout << "mt_rand(" << min << ", " << max << ") = " << ret << std::endl;
#endif
  return ret;
}

mt_rand::mt_rand(int seed) {
  generator = new PHP_mt19937;
  ((PHP_mt19937 *)generator)->seed(seed);
}

mt_rand::~mt_rand() {
  if (generator) {
    delete (PHP_mt19937 *)generator;
  }
}
