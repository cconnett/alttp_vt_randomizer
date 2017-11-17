#include "mt_rand.h"

int mt_rand(int low, int high) { return 6; }
void mt_shuffle(int n, int *array) {
  for (unsigned int i = n - 1; i > 0; i--) {
    int j = mt_rand(0, i);
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
  }
}
