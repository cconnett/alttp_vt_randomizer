#include <cassert>
#include <vector>

uint mt_rand(uint low, uint high);
void php_srand(int seed);

template <typename T>
void mt_shuffle(T *array, size_t n) {
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
  std::vector<T> original, ret;
  for (uint i = 0; i < popsize; i++) {
    original.push_back(population[i]);
  }
  while (k-- > 0) {
    uint pos = mt_rand(0, original.size());
    ret.push_back(original[pos]);
    original.erase(original.begin() + pos);
  }
  return ret;
}
