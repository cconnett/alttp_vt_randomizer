uint mt_rand(uint low, uint high);

template <typename T>
void mt_shuffle(uint n, T *array) {
  for (uint i = n - 1; i > 0; i--) {
    uint j = mt_rand(0, i);
    T tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
  }
}
