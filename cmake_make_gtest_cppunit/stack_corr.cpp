#include <math.h>
#include <stdlib.h> /* srand, rand */
#include <sys/resource.h>
#include <time.h> /* time */
#include <cstdio>
#include <iostream>

void shell_sort(long* my_array, long length) {
  int h = 0;
  for (h = 1; h <= length / 9; h = 3 * h + 1)
    ;

  for (; h > 0; h /= 3) {
    for (int i = h; i < length; ++i) {
      long j = i;
      long tmp = my_array[i];
      while (j >= h && tmp < my_array[j - h]) {
        my_array[j] = my_array[j - h];
        j -= h;
      }
      my_array[j] = tmp;
    }
  }
}

int main(int argc, char* argv[]) {
  long arr[32];
  size_t i = 0;
  srand(time(NULL));
  for (; i < rand() % sizeof arr; ++i) {
    arr[i] = rand();
  }
  shell_sort(arr, 32);

  char str[256];
  strcpy(str, "Some string");
  // p str
  // x str
  printf("%s", str);

  char* str_dyn = (char*)malloc(256 * sizeof(char));
  strcpy(str_dyn, "Some string");
  printf("%s", str_dyn);

  return 0;
}