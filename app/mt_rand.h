#include <vector>

uint mt_rand(uint low, uint high);

template <typename T>
void mt_shuffle(size_t n, T *array) {
  for (uint i = n - 1; i > 0; i--) {
    uint j = mt_rand(0, i);
    T tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
  }
}

template <typename T>
std::vector<T> mt_sample(size_t b, size_t n, T *array) {
  std::vector<T> original, ret;
  for (uint i = 0; i < n; i++) {
    original.push_back(array[i]);
  }
  while (b-- > 0) {
    uint pos = mt_rand(0, original.size());
    ret.push_back(original[pos]);
    original.erase(original.begin() + pos);
  }
  return ret;
}
