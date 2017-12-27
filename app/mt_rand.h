#ifndef _MT_RAND_H
#define _MT_RAND_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

unsigned int mt_rand(uint low, uint high);
void mt_srand(int seed);

using namespace std;

// Adapted from Helpers/array.php:mt_shuffle
template <typename T>
void mt_shuffle(T *array, size_t n) {
  uint n2 = n;
  T old[n2];
  memcpy(old, array, sizeof(old));
  int out = 0;
  while (n2) {
    uint j = mt_rand(0, n2 - 1);
    array[out++] = old[j];
    while (j < n2 - 1) {
      old[j] = old[j + 1];
      j++;
    }
    n2--;
  }
}

template <typename T>
void knuth_shuffle(T *array, size_t n) {
  for (uint i = n - 1; i > 0; i--) {
    uint j = mt_rand(0, i);
    T tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
  }
}

template <typename T>
std::vector<T> mt_sample(T *population, size_t popsize, size_t k) {
  assert(k <= popsize);
  std::vector<T> urn, ret;
  for (unsigned int i = 0; i < popsize; i++) {
    urn.push_back(population[i]);
  }
  while (k-- > 0) {
    unsigned int pos = mt_rand(0, urn.size() - 1);
    ret.push_back(urn[pos]);
    urn.erase(urn.begin() + pos);
  }
  return ret;
}

#endif
