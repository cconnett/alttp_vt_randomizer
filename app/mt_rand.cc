#include <iomanip>
#include <iostream>

#include "mt_rand.h"
#include "prngs/Mt19937.h"

uint scale(uint n, uint min, uint max) {
  return min + (uint)((max - min + 1.0) * (((double)n) / (uint)0xffffffff));
}

unsigned int mt_rand::rand(uint min, uint max) {
  max -= min;
  max++;

  uint result = ((Mt19937 *)generator)->random();
  if ((max & (max - 1)) == 0) {
    return result & (max - 1);
  }
  uint limit = 0xffffffff - (0xffffffff % max) - 1;
  while (result > limit) {
    result = ((Mt19937 *)generator)->random();
  }
#ifdef RAND_SYNC
  std::cout << "mt_rand(" << min << ", " << max + min
            << ") = " << result % max + min << std::endl;
#endif
  return result % max + min;
}

mt_rand::mt_rand(int seed) {
  generator = new Mt19937;
  ((Mt19937 *)generator)->seed(seed);
}

mt_rand::~mt_rand() {
  if (generator) {
    delete (Mt19937 *)generator;
  }
}
