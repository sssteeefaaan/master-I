#include "matrix_utils.h"

int main(int argc, char** argv){
    LL N = 1 << 3,
        M = 1 << 5;

    switch(argc){
        case(3): M = atoll(argv[2]);
        case(2): N = atoll(argv[1]);
    }

    float* par, *seq;
    double dt;

    initialize_vector_random(&seq, N * M);
    copy_vector(&par, seq, N * M);

    dt = omp_get_wtime();
    #pragma omp parallel
    transpose_matrix(&par, N, M);
    dt = omp_get_wtime() - dt;

    dt = omp_get_wtime();
    transpose_matrix(&seq, N, M);
    dt = omp_get_wtime() - dt;

    compare_vectors(par, seq, N * M)
    ?
    printf("Correct!\n")
    :
    printf("False!\n");

    free_vector(&seq);
    free_vector(&par);

    return 0;
}