#include "utils.h"

int main(int argc, char** argv){
    LL N = 1 << 3,
    M = 1 << 4,
    K = 1 << 5;

    switch(argc){
        case(4):
            K = atoll(argv[3]);
        case(3):
            M = atoll(argv[2]);
        case(2):
            N = atoll(argv[1]);
    }

    float   **mat1 = NULL,
            **mat2 = NULL,
            **par = NULL,
            **seq = NULL;

    double dt;
    
    initialize_matrix_random(&mat1, N, M);
    initialize_matrix_random(&mat2, M, K);
    initialize_matrix_empty(&par, N, K);
    initialize_matrix_empty(&seq, N, K);
    
    dt = omp_get_wtime();
    #pragma omp parallel
    multiply_matrices(mat1, mat2, par, N, M, K);
    dt = omp_get_wtime() - dt;
    printf("Parallel time taken: %fms\n", dt);

    dt = omp_get_wtime();
    multiply_matrices(mat1, mat2, seq, N, M, K);
    dt = omp_get_wtime() - dt;
    printf("Sequential time taken: %fms\n", dt);

    compare_matrices(seq, par, N, K) == 0
    ?
    printf("Correct!\n")
    :
    printf("False!\n");

    free_matrix(&seq, N);
    free_matrix(&par, N);
    free_matrix(&mat2, M);
    free_matrix(&mat1, N);

    return 0;
}