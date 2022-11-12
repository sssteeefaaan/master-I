#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MSIZE 64
//#define PRINT

int main(int argc, char *argv[]) {

  int *randomMatrix = (int *) calloc(MSIZE * MSIZE * sizeof(int));

  clock_t t1, t2;
  t1 = clock();
  int i, j;
  #pragma acc kernels
  {
      for(i = 0; i < MSIZE; i++)
          for(j = 0; j < MSIZE; j++)
              randomMatrix[i * MSIZE + j] = randomMatrix[i * MSIZE + j] + 2;
  }
  t2 = clock();

#ifdef PRINT
  for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        printf("matrica[%d,%d] = %d\n", i, j, randomMatrix[i * MSIZE + J);
      }
  }
#endif

  free(randomMatrix);
  printf("Elapsed Time: %f ms\n",((float)(t2 - t1) / 1000000.0F ) * 1000);

  return 0;
}
