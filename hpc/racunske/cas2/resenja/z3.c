#include "binary_tree_utils.h"

int main(int argc, char** argv){
    LL N = 1 << 4;

    switch (argc)
    {
        case (2):
            N = atoll(argv[1]);
    }

    struct tree *t;
    float par, seq;
    double dt;
    
    initialize_tree_random(&t, N);
    printf("Initialization done!\n");

    dt = omp_get_wtime();
    #pragma omp parallel
    #pragma omp single
    par = sum_tree_node(t);
    dt = omp_get_wtime() - dt;
    printf("Parallel time taken: %f\n", dt);
    
    dt = omp_get_wtime();
    seq = sum_tree_node(t);
    dt = omp_get_wtime() - dt;
    printf("Sequential time taken: %f\n", dt);

    par == seq
    ?
    printf("Correct!\n")
    :
    printf("False!\n");

    free_tree(&t);

    return 0;
}