#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int
        PING_PONG_SIZE = 9,
        PLAYER_NUMBER = 2;

    if(size - 1 != PLAYER_NUMBER){
        printf("Cannot play ping pong with %d players! Player number needs to be %d (+1 for the judge role)!\n", size - 1, PLAYER_NUMBER);
        exit(-1);
    }

    const int message_length = 100;
    char message[message_length];
    int
        ping_pongs = 0,
        partner = (rank + 1) % PLAYER_NUMBER;
    MPI_Status status;

    while(ping_pongs< PING_PONG_SIZE)
    {
        memset(message, '\0', sizeof(char) * message_length);
        if(rank == ping_pongs % PLAYER_NUMBER)
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
            sprintf(message, "(%d): [%d] ---%s--- [%d]", ping_pongs, rank, ping_pongs % 2 == 1 ? "PING" : "PONG", partner);
            MPI_Send(
                &message,
                message_length,
                MPI_CHAR,
                size - 1,
                ping_pongs,
                MPI_COMM_WORLD
            );
        }
        else if(rank == (ping_pongs + 1) % PLAYER_NUMBER)
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
        }
        else if(rank == size - 1)
        {
            ping_pongs++;
            MPI_Recv(
                &message,
                message_length,
                MPI_CHAR,
                (ping_pongs - 1) % PLAYER_NUMBER,
                ping_pongs,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );
            printf("%s\n", message);
        }
    }

    MPI_Finalize();
    
    return 0;
}
