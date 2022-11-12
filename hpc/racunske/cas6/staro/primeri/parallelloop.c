#include <stdlib.h>
#include <openacc.h>

#define N 1024
#define M 1024

int main() {

  float *A = (float *) malloc(N * M * sizeof(float));

  #pragma acc parallel loop gang
  for (int i=0; i<N; i++)
      #pragma acc loop vector
      for (int j=0; j<M; j++)
        A[i * N + j] = 1.f;
    
  free(A);

  return 0;
}