#include "mt_rand.h"

int scale(int n, int min, int max) {
  return (int)(min + (max - min + 1.0) * (((double)n) / (1 << 31)));
}

int mt_rand(int min, int max) { return scale(19283, min, max); }

void mt_shuffle(int n, int *array) {
  for (unsigned int i = n - 1; i > 0; i--) {
    int j = mt_rand(0, i);
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
  }
}
