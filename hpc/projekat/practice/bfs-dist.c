#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

const int MASTER = 0;
const int INFINITY = INT16_MAX;

int compare_vectors(int* a, int* b, int n){
    int t = 1;
    for(int i = 0; t && i < n; i++) t = a[i] == b[i];
    return t;
}

void swap(int** a, int** b){
    int* temp = *a;
    *a = *b;
    *b = temp;
}

void bfs_seq(int* graph, int vertex_numb, int degree, int start, int* distance)
{
    for(int i = 0; i < vertex_numb; i++)
        distance[i] = INFINITY;
    distance[start] = 0;

    int *fs = (int*) malloc(sizeof(int) * vertex_numb),
        *ns = (int*) malloc(sizeof(int) * vertex_numb);

    fs[0] = start;

    int level = 1,
        count1 = 1,
        count2 = 0;

    while(count1 > 0)
    {
        for(int i = 0; i < count1; i++){
            int node = fs[i];
            for(int j = 0; j < degree; j++){
                int neighbour = graph[node * degree + j];
                if(distance[neighbour] == INFINITY){
                    distance[neighbour] = level;
                    ns[count2++] = neighbour;
                }
            }
        }
        count1 = count2;
        count2 = 0;
        swap(&fs, &ns);
        level++;
    }

    free(ns);
    free(fs);
}

void bfs_dist(int* graph, int vertex_numb, int degree, int start, int* distance)
{
    int rank, size, work_load;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    work_load = vertex_numb / size + 1;

    MPI_Bcast(graph, vertex_numb * degree, MPI_INT, MASTER, MPI_COMM_WORLD);
    int *d = (int*) malloc(sizeof(int) * vertex_numb);
    for(int i = 0; i < vertex_numb; i++) d[i] = INFINITY;
    d[start] = 0;

    int *F = (int*)malloc(sizeof(int) * vertex_numb),
        *N = (int*) malloc(sizeof(int) * size * vertex_numb),
        *N_recv = (int*) malloc(sizeof(int) * vertex_numb),
        *N_size = (int*) malloc(sizeof(int) * size);

    int level = 0, F_count, F_global_count, my_size;
    while(1){
        F_count = 0;
        for(int i = 0; i < work_load; i++){
            int index = rank * work_load + i;
            if(index < vertex_numb && d[index] == level){
                F[F_count++] = index;
            }
        }
        MPI_Allreduce(&F_count, &F_global_count, 1, MPI_INT, MPI_BOR, MPI_COMM_WORLD);
        if(F_global_count == 0)
            break;

        for(int i = 0; i < size; i++)
            N_size[i] = 0;

        for(int i = 0; i < F_count; i++)
        {
            int node = F[i];
            for(int j = 0; j < degree; j++){
                int neighbour = graph[node * degree + j],
                    proc = neighbour / work_load,
                    k = 0;
                while(k < N_size[proc] && N[proc * vertex_numb + k] != neighbour) k++;
                if(k == N_size[proc]) N[proc * vertex_numb + (N_size[proc]++)] = neighbour;
            }
        }

        for(int i = 0; i < size; i++)
        {
            MPI_Sendrecv(&N_size[i], 1, MPI_INT, i, level, &my_size, 1, MPI_INT, i, level, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Sendrecv(&N[i * vertex_numb], N_size[i], MPI_INT, i, level, N_recv, my_size, MPI_INT, i, level, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for(int j = 0; j < my_size; j++)
                if(d[N_recv[j]] == INFINITY)
                    d[N_recv[j]] = level + 1;
        }
        level++;
    }

    MPI_Reduce(d, distance, vertex_numb, MPI_INT, MPI_MIN, MASTER, MPI_COMM_WORLD);

    free(d);
    free(N_size);
    free(N_recv);
    free(N);
    free(F);
}

void generate_random_graph(int* graph, int vertex_numb, int degree)
{
    for(int i = 0; i < vertex_numb; i++){
        for(int j = 0; j < degree; j++){
            int val;
            do{
                val = rand() % vertex_numb;
            }while(val == i);

            int k = 0;
            while(val != graph[i * degree + k] && k < degree) k++;
            if(k == degree)
                graph[i * degree + j] = val;
            else j--;
        }
    }
}

void print_graph(int*G, int vertex_numb, int degree)
{
    for(int i = 0; i < vertex_numb; i++)
    {
        printf("%d | ", i);
        for(int j = 0; j < degree; j++)
        {
            printf("%d ", G[i * degree + j]);
        }
        printf("|\n");
    }
}

void print_distance(int*d, int vertex_numb, int start)
{
    printf("distance(%d) = |\t ", start);
    for(int i = 0; i < vertex_numb; i++)
        printf("%d\t ", d[i]);
    printf("|\n");
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int vertex_numb = 1000000,
    degree = 1,
    start_node = 0;

    switch(argc){
        case(4): start_node = atoi(argv[3]);
        case(3): degree = atoi(argv[2]);
        case(2): vertex_numb = atoi(argv[1]);
    }

    double dt;

    int * G = (int*)malloc(sizeof(int) * vertex_numb * degree),
        * d_d = (int*) malloc(sizeof(int) * vertex_numb);

    if(rank == MASTER){
        printf("vertex_numb = %d\ndegree = %d\nstart_node = %d\n", vertex_numb, degree, start_node);
        generate_random_graph(G, vertex_numb, degree);
        dt = MPI_Wtime();
    }

    bfs_dist(G, vertex_numb, degree, start_node, d_d);

    if(rank == MASTER){
        dt = MPI_Wtime() - dt;
        printf("[Distributed]: Time taken %f\n", dt);

        int* d_s = (int*) malloc(sizeof(int) * vertex_numb);

        dt = MPI_Wtime();
        bfs_seq(G, vertex_numb, degree, start_node, d_s);
        dt = MPI_Wtime() - dt;
        printf("[Sequential]: Time taken %f\n", dt);

        compare_vectors(d_d, d_s, vertex_numb) ? printf("Correct!\n") : printf("False!\n");

        free(d_s);
    }

    free(d_d);
    free(G);

    MPI_Finalize();

    return 0;
}