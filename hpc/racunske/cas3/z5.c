#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ima prekide zbog neblokirajucih funkcija za veliko PING_PONG_SIZE
// Razlog: no idea

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int
        PING_PONG_SIZE = 9,
        NUMBER_OF_PLAYERS = 2;

    if(size != NUMBER_OF_PLAYERS + 1)
    {
        printf("Player number cannot be %d! Number of players needs to be %d (+1 for the judge role)!\n", size, NUMBER_OF_PLAYERS);
        exit(-1);
    }

    const int message_length = 100;
    char message[message_length];
    int
        ping_pongs = 0,
        partner = (rank + 1) % NUMBER_OF_PLAYERS;
    short has_sent_pp = 0, has_sent_msg = 0;
    MPI_Request request_pp, request_msg;
    MPI_Status status_pp, status_msg;

    while(ping_pongs< PING_PONG_SIZE)
    {
        memset(message, '\0', sizeof(char) * message_length);
        if(rank == ping_pongs % NUMBER_OF_PLAYERS)
        {
            ping_pongs++;
            if(has_sent_pp) MPI_Wait(&request_pp, MPI_STATUS_IGNORE);
            MPI_Isend(
                &ping_pongs,
                1,
                MPI_INT,
                partner,
                0,
                MPI_COMM_WORLD,
                &request_pp
            );
            has_sent_pp = 1;
            sprintf(message, "(%d): [%d] ---%s--- [%d]", ping_pongs, rank, ping_pongs % 2 == 1 ? "PING" : "PONG", partner);
            if(has_sent_msg == 1) MPI_Wait(&request_msg, &status_msg);
            MPI_Isend(
                &message,
                strlen(message) + 1,
                MPI_CHAR,
                size - 1,
                ping_pongs,
                MPI_COMM_WORLD,
                &request_msg
            );
            has_sent_msg = 1;
        }
        else if(rank == (ping_pongs + 1) % NUMBER_OF_PLAYERS)
        {
            MPI_Recv(
                &ping_pongs,
                1,
                MPI_INT, 
                partner,
                0,
                MPI_COMM_WORLD,
                &status_pp
            );
        }
        else if(rank == size - 1)
        {
            ping_pongs++;
            MPI_Recv(
                &message,
                message_length,
                MPI_CHAR,
                (ping_pongs - 1) % NUMBER_OF_PLAYERS,
                ping_pongs,
                MPI_COMM_WORLD,
                &status_msg
            );
            printf("%s\n", message);
        }
    }

    MPI_Finalize();
    
    return 0;
}
