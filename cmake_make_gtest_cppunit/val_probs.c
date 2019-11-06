#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void bad_func(char *p) {
  p[4096] = 'a';
  p[8096] = 'b';
  p[16096] = 'c';
}

typedef struct Tree {
  int value;
  struct Tree *left;
  struct Tree *right;
} Tree;

void good_work() { printf("good...\n"); }

void bad_work() {
  // 1. directly lost
  if (1) {
    char *p = malloc(8);
    // free(p);
  }

  // 2. indirectly lost
  if (1) {
    Tree *root = malloc(sizeof(Tree));
    root->left = malloc(sizeof(Tree));  // <-- indirectly
    root->right = malloc(sizeof(Tree)); // <-- indirectly
                                        // free(root);
  }

  // 3. possilble lost
  if (0) {
    char *p = malloc(4);
    ++p; // I know I can do '--p' and then free(p), but valgrind doesn't know
         // that.

    char tmp[8];
    bad_func(tmp);

    // or kill(getpid(), SIGKILL);
    --p;
    free(p);
  }

  // 4. still reachable, do wired things after you allocate!
  if (0) {
    char *p = malloc(4);
    kill(getpid(), SIGKILL);

    // or smth that causes segfault, e.g.
    // char *s = "string";
    // *s = 'S';
    free(p);
  }

  // 5. Conditional jump or move depends on uninitialised value(s)
  if (0) {
    // int r;
    // int size = r + 4;
    // printf("r: %d\n", size);

    char *data = malloc(5);
    data[0] = 'a';
    data[1] = 'b';
    data[1] = 'c';
    if (data[2] == 'c') {
      printf("YES\n");
    }
    free(data);
  }

  // 6. invalid read/write
  if (0) {
    // int *arr = malloc(8*sizeof(int));
    int *arr = malloc(8);
    int a = arr[0];
    int b = arr[1];
    int c = arr[2];
    free(arr);

    // and double free() also !
    // free(arr);
  }
}

int main(int argc, char *argv[]) {
  printf("Hello, TechnoPark!\n");
  int n = 5;
  while (n--) {
    // good_work();
    printf("n = %d\n", n);
    bad_work();
    sleep(1);
  }

  return 0;
}
