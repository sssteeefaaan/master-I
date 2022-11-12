// Hello World iz hibridnog OpenMPI + OpenMP programa.

#include <stdio.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    #pragma omp parallel
    {
        printf("Hello World iz procesa %d i niti %d.\n", rank, omp_get_thread_num());
    }

    MPI_Finalize();

    return 0;
}