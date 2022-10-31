#include <mpi.h>
#include <stdio.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm new_comm;

    MPI_Comm_split(
        MPI_COMM_WORLD,
        rank % 2,
        rank,
        &new_comm
    );

    int new_rank, new_size;
    MPI_Comm_rank(new_comm, &new_rank);
    MPI_Comm_size(new_comm, &new_size);

    printf("MPI_COMM_WORLD_rank: %d/%d - new_comm_rank: %d/%d\n", rank, size, new_rank, new_size);

    MPI_Finalize();
    return 0;
}