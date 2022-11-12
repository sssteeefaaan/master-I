#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MATRIX_SIZE 10000

int main() {
    int *randomMatrix = (int *) malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));

    //Set the seed
    srand(time(NULL));
    for(int i = 0; i < MATRIX_SIZE; i++)
        for(int j = 0; j < MATRIX_SIZE; j++)
            randomMatrix[i * MATRIX_SIZE + j] = rand() % 100;

    clock_t t1, t2;
    t1 = clock();
    int i, j;
    #pragma acc kernels
    {
        for(i = 0; i < MATRIX_SIZE; i++)
            for(j = 0; j < MATRIX_SIZE; j++)
                randomMatrix[i * MATRIX_SIZE + j] = randomMatrix[i * MATRIX_SIZE + j] * 2;
    }
    t2 = clock();

    free(randomMatrix);
    printf("Elapsed Time: %f ms\n",((float)(t2 - t1) / 1000000.0F ) * 1000);
    return 0;
}