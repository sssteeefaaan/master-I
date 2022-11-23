#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

const int A = 0;
const int B = 1;
const int C = 2;

const int M1 = 0;
const int M2 = 1;
const int M3 = 2;
const int M4 = 3;
const int M5 = 4;
const int M6 = 5;
const int M7 = 6;

const char processes[] = {'A', 'B', 'C'};

int rank, size, timestamp_counter = 0, id = 1;

typedef struct{
    int timestamp;
    int rank;
    int content;
} message;

void send(int, int, int, int*);
void receive(int, int, int*);


int main(int argc, char** argv){

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    switch(rank){
        case(0):
            send(rank, B, M1, &timestamp_counter);
            receive(A, M5, &timestamp_counter);
            send(A, B, M6, &timestamp_counter);
            receive(A, M4, &timestamp_counter);
            receive(A, M7, &timestamp_counter);
            break;
        case(1):
            receive(B, M2, &timestamp_counter);
            receive(B, M1, &timestamp_counter);
            send(B, C, M3, &timestamp_counter);
            send(B, A, M4, &timestamp_counter);
            receive(B, M6, &timestamp_counter);
            send(B, A, M7, &timestamp_counter);
            break;
        case(2):
            send(rank, B, M2, &timestamp_counter);
            receive(C, M3, &timestamp_counter);
            send(C, A, M5, &timestamp_counter);
            break;
        default:
            break;
    }

    MPI_Finalize();

}


void send(int my_rank, int to_rank, int tag, int* timestamp_counter)
{
    (*timestamp_counter)++;
    message m;
    m.content = 0;
    m.rank = my_rank;
    m.timestamp = *timestamp_counter;

    printf("(%c) --M%d--> (%c): timestamp(%d, %d)\n", processes[my_rank], tag + 1, processes[to_rank], *timestamp_counter, id++);
    fflush(stdout);

    MPI_Send(&m, 3, MPI_INT, to_rank, tag, MPI_COMM_WORLD);
}

void receive(int my_rank, int tag, int* timestamp_counter)
{
    message ret;
    MPI_Status status;
    MPI_Recv(&ret, 3, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
    if(*timestamp_counter < ret.timestamp)
        *timestamp_counter = ret.timestamp;
    (*timestamp_counter)++;

    printf("(%c) <--M%d-- (%c): timestamp(%d, %d)\n", processes[my_rank], tag + 1, processes[status.MPI_SOURCE], *timestamp_counter, id++);
    fflush(stdout);
}