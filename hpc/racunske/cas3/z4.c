#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int token,
        source = rank - 1 < 0 ? size - 1 : rank - 1;

    if(!rank)
        token = -210;
    else
        MPI_Recv(
            &token,
            1,
            MPI_INT,
            source,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

    MPI_Send(
        &token,
        1,
        MPI_INT,
        (rank + 1) % size,
        0,
        MPI_COMM_WORLD
    );

    if(!rank)
        MPI_Recv(
            &token,
            1,
            MPI_INT,
            source,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

    printf("P[%d]: Recieved (%d) from P[%d].\n", rank, token, source);

    MPI_Finalize();

    return 0;
}