#include "queue.h"
#include <mpi.h>
#include <signal.h>
#include <unistd.h>

const int COORDINATOR = 0;
const int REQUEST = 1;
const int RELEASE = 2;
const int OK = 4;

int OCCUPIED = 0;
int WORK = 1;

void coordinator_work(queue* q){
    int buf, source;
    MPI_Request req;
    MPI_Status status;

    for(;WORK;){
        MPI_Irecv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &req);

        while(WORK){
            int done = 0;
            MPI_Test(&req, &done, &status);
            if(done)
                break;
        }

        source = status.MPI_SOURCE;

        if (buf == REQUEST){
            if (!OCCUPIED){
                OCCUPIED = 1;//!OCCUPIED;
                buf = OK;
                MPI_Send(&buf, 1, MPI_INT, source, 0, MPI_COMM_WORLD);

                printf("P[%d] (COORDINATOR): %d requested -> %d accepted!\n", COORDINATOR, source, source);
            }else{
                enqueue(q, source);
                printf("P[%d] (COORDINATOR): %d requested -> %d enqueued!\n", COORDINATOR, source, source);
            }
        }
        else if (buf == RELEASE){
            printf("P[%d] (COORDINATOR): %d released -> ", COORDINATOR, source);
            if (!is_empty(q)){
                source = dequeue(q);
                buf = OK;
                printf("%d accepted!\n", source);
                MPI_Send(&buf, 1, MPI_INT, source, 0, MPI_COMM_WORLD);
            }else{
                OCCUPIED = 0;//!OCCUPIED;
                printf("resource is free!\n");
            }
        }

        fflush(stdout);
    }

    printf("Coordinator exited!\n");
    fflush(stdout);
}

int request(){
    int message = REQUEST;
    MPI_Status status;

    MPI_Send(&message, 1, MPI_INT, COORDINATOR, 0, MPI_COMM_WORLD);
    MPI_Recv(&message, 1, MPI_INT, COORDINATOR, 0, MPI_COMM_WORLD, &status);

    return message;
}

void release(){
    int message = RELEASE;

    MPI_Send(&message, 1, MPI_INT, COORDINATOR, 0, MPI_COMM_WORLD);
}

void worker_work(int rank){
    printf("P[%d]: Waiting...\n", rank);
    fflush(stdout);

    request();

    printf("P[%d]: Doing work...\n", rank);
    fflush(stdout);

    sleep(2);

    release();

    printf("P[%d]: Done!\n", rank);
    fflush(stdout);
}

void handler(int s){
    WORK = 0;
}

int main(int argc, char** argv){

    signal(SIGINT, handler);

    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    queue* q = initialize(size);

    if (rank == COORDINATOR){
        coordinator_work(q);
    }else{
        worker_work(rank);
    }

    MPI_Finalize();
}