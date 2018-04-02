#include <iomanip>
#include <iostream>

#include "mt_rand.h"

// PHP 7.0 has the bad algorithm and bad scaling.
// PHP 7.1 has the bad algorithm and optional bad scaling - v28 passed that
// option, v29 doesn't.
// PHP 7.2 has the good algorithm and good scaling.
#define BADGENERATOR
#define BADSCALING

#ifdef BADGENERATOR
#include "prngs/PHP_mt19937.h"
typedef PHP_mt19937 Generator;
#else
#include "prngs/Mt19937.h"
typedef Mt19937 Generator;
#endif

#ifdef BADSCALING
unsigned int mt_rand::rand(uint min, uint max) {
  uint raw = ((Generator *)generator)->random();
  uint ret = min + (uint)((max - min + 1.0) * (((double)raw) / 0x80000000ul));
#ifdef RAND_SYNC
  std::cout << "mt_rand(" << min << ", " << max + min << ") = " << ret
            << std::endl;
#endif
  assert(min <= ret);
  assert(ret <= max);
  return ret;
}
#else
unsigned int mt_rand::rand(uint min, uint max) {
  max -= min;
  max++;

  uint result = ((Generator *)generator)->random();
  if ((max & (max - 1)) == 0) {
#ifdef RAND_SYNC
    std::cout << "mt_rand(" << min << ", " << max + min - 1
              << ") = " << (result & (max - 1)) << std::endl;
#endif
    return result & (max - 1);
  }
  uint limit = 0xffffffff - (0xffffffff % max) - 1;
  while (result > limit) {
    result = ((Generator *)generator)->random();
  }
#ifdef RAND_SYNC
  std::cout << "mt_rand(" << min << ", " << max + min - 1
            << ") = " << result % max + min << std::endl;
#endif
  return result % max + min;
}
#endif

mt_rand::mt_rand(int seed) {
  generator = new Generator;
  ((Generator *)generator)->seed(seed);
}

mt_rand::~mt_rand() {
  if (generator) {
    delete (Generator *)generator;
  }
}
