#include <cassert>
#include <cstdlib>
#include <vector>

unsigned int mt_rand(uint low, uint high);
void mt_srand(int seed);

template <typename T>
void mt_shuffle(T *array, size_t n) {
  T old[n];
  memcpy(old, array, sizeof(old));
  int out = 0;
  while (n--) {
    uint j = mt_rand(0, n);
    array[out++] = old[j];
    while (j < n) {
      old[j] = old[j + 1];
      j++;
    }
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
