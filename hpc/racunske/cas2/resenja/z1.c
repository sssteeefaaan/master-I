#include "matrix_utils.h"

int main(int argc, char**argv){
    LL N = 1 << 3,
        M = 1 << 3;

    switch(argc){
        case(3):
            M = atoll(argv[2]);
        case(2):
            N = atoll(argv[1]);
        default:
        break;
    }

    float**matrix = NULL,
        *vector = NULL;

    initialize_vector_random(&vector, M);
    initialize_matrix_random(&matrix, N, M);

    float* par = (float*) malloc(sizeof(float) * N);
    double dt = omp_get_wtime();
    #pragma omp parallel
    multiply(matrix, N, M, vector, par);
    dt = omp_get_wtime() - dt;
    printf("Parallel time taken: %fms\n", dt);

    float * seq = (float*) malloc(sizeof(float) * N);
    dt = omp_get_wtime();
    multiply(matrix, N, M, vector, seq);
    dt = omp_get_wtime() - dt;
    printf("Sequential time taken: %fms\n", dt);

    compare_vectors(par, seq, N) == 0
    ?
    printf("Correct!\n")
    :
    printf("False!\n");

    free_vector(&seq);
    free_vector(&par);
    free_vector(&vector);
    free_matrix(&matrix, N);

    return 0;
}