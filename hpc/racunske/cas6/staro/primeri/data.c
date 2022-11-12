#include <stdlib.h>

#define N 1024

int main() {

  float *x = (float *) malloc(sizeof(float) * N);
  float *y = (float *) malloc(sizeof(float) * N);

  #pragma acc data
  {
    #pragma acc parallel loop
    for (int i=0; i<N; i++) {
      y[i] = 0.0f;
      x[i] = (float)(i+1);
    }
    #pragma acc parallel loop
    for (int i=0; i<N; i++) {
      y[i] = 2.0f * x[i] + y[i];
    }
  }

  free(x); free(y);

  return 0;
}