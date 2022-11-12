#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openacc.h>

#define MSIZE 64
//#define PRINT

int main(int argc, char *argv[]) {
  
  int *randomMatrix = (int *) malloc(MSIZE * MSIZE * sizeof(int));

  clock_t t1, t2;
  t1 = clock();
  #pragma acc kernels
  {
    for(int i = 0; i < MSIZE; i++) {
        for(int j = 0; j < MSIZE; j++) {
            if ((i + j) % 2) {
                randomMatrix[i * MSIZE + j] = 1;
            } else {
                randomMatrix[i * MSIZE + j] = 0;
            }
        }
      }
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
