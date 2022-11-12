#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int size, rank;

    const int
        PING_PONG_MAX = 10,
        PLAYERS_NUMB = 2;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(size != PLAYERS_NUMB){
        printf("Can't play ping pong with %d players! Player number needs to be %d!\n", size, PLAYERS_NUMB);
        exit(-1);
    }

    int ping_pongs = 0,
        partner = (rank + 1) % PLAYERS_NUMB;

    while(ping_pongs < PING_PONG_MAX)
    {
        if(ping_pongs % PLAYERS_NUMB == rank)
        {
            ping_pongs++;
            MPI_Send(
                &ping_pongs,
                1,
                MPI_INT,
                partner,
                0,
                MPI_COMM_WORLD
            );
        }
        else
        {
            MPI_Recv(
                &ping_pongs,
                1,
                MPI_INT,
                partner,
                0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );
            printf("p%d received ping_pong_count %d from p%d.\n", rank, ping_pongs, partner);
        }
    }

    MPI_Finalize();

    return 0;
}