#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

const long long MASTER = 0;
const long long INFINITY = 0xffff;

void print_vector(char* label, long long*v, long long n)
{
    printf("%s\t |", label);
    for(long long i = 0; i < n; i++)
        printf("\t %d", v[i]);
    printf("\t|\n");
}

long long compare_vectors(long long* a, long long* b, long long n){
    long long t = 1;
    for(long long i = 0; t && i < n; i++) t = a[i] == b[i];
    return t;
}

void swap(long long** a, long long** b){
    long long* temp = *a;
    *a = *b;
    *b = temp;
}

void bfs_seq(
    long long* graph,
    long long* degrees,
    long long vertex_numb,
    long long start,
    long long* distance
){
    for(long long i = 0; i < vertex_numb; i++)
        distance[i] = INFINITY;
    distance[start] = 0;

    long long *fs = (long long*) malloc(sizeof(long long) * vertex_numb),
        *ns = (long long*) malloc(sizeof(long long) * vertex_numb);

    fs[0] = start;

    long long level = 0,
        count1 = 1,
        count2 = 0;

    while(count1 > 0)
    {
        for(long long i = 0; i < count1; i++){
            long long node = fs[i];
            for(long long j = degrees[node]; j < degrees[node + 1]; j++){
                long long neighbour = graph[j];
                if(distance[neighbour] == INFINITY){
                    distance[neighbour] = level + 1;
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

void bfs_dist(
    long long* graph,
    long long* degrees,
    long long vertex_numb,
    long long start,
    long long* distance
){
    int rank, size;
    long long work_load;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    work_load = vertex_numb / size + 1;

    MPI_Bcast(degrees, vertex_numb + 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
    if(rank)
        graph = (long long*) malloc(sizeof(long long) * degrees[vertex_numb]);
    MPI_Bcast(graph, degrees[vertex_numb], MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);

    long long *d = (long long*) malloc(sizeof(long long) * vertex_numb);
    for(long long i = 0; i < vertex_numb; i++)
        d[i] = INFINITY;
    d[start] = 0;

    long long *F = (long long*) malloc(sizeof(long long) * vertex_numb),
        *N = (long long*) malloc(sizeof(long long) * size * vertex_numb),
        *N_recv = (long long*) malloc(sizeof(long long) * vertex_numb),
        *N_size = (long long*) malloc(sizeof(long long) * size);

    long long level = 0,
        F_count,
        F_global_count,
        my_size;

    while(1) {

        F_count = F_global_count = 0;

        for(long long i = rank * work_load; i < (rank + 1) * work_load; i++)
            if(i < vertex_numb && d[i] == level)
                F[F_count++] = i;

        MPI_Allreduce(&F_count, &F_global_count, 1, MPI_LONG_LONG, MPI_BOR, MPI_COMM_WORLD);

        if(F_global_count == 0)
            break;

        for(long long i = 0; i < size; i++)
            N_size[i] = 0;

        for(long long i = 0; i < F_count; i++)
        {
            long long current_node = F[i];

            for(long long j = degrees[current_node]; j < degrees[current_node + 1]; j++)
            {
                long long neighbour_node = graph[j],
                    proc = neighbour_node / work_load,
                    k = 0;

                while(k < N_size[proc] && N[proc * vertex_numb + k++] != neighbour_node);

                if(k == N_size[proc])
                    N[proc * vertex_numb + N_size[proc]++] = neighbour_node;
            }
        }

        for(int i = 0; i < size; i++)
        {
            MPI_Sendrecv(&N_size[i], 1, MPI_LONG_LONG, i, level, &my_size, 1, MPI_LONG_LONG, i, level, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Sendrecv(&N[i * vertex_numb], N_size[i], MPI_LONG_LONG, i, level, N_recv, my_size, MPI_LONG_LONG, i, level, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for(long long j = 0; j < my_size; j++)
                if(d[N_recv[j]] == INFINITY)
                    d[N_recv[j]] = level + 1;
        }

        level++;
    }

    MPI_Reduce(d, distance, vertex_numb, MPI_LONG_LONG, MPI_MIN, MASTER, MPI_COMM_WORLD);

    free(d);
    free(N_size);
    free(N_recv);
    free(N);
    free(F);
}

void reset_options(long long *options, long long size){
    for(long long i = 0; i < size; i++)
        options[i] = i;
}

long long get_unique_random(long long *options, long long maximum){
    long long cursor = rand() % maximum,
    tmp = options[cursor];
    options[cursor] = options[maximum - 1];
    return tmp;
}

void generate_random_graph(
    long long** graph,
    long long* degrees,
    long long vertex_numb,
    long long max_degrees,
    long long min_degrees
){
    long long max_deg = vertex_numb < max_degrees ? vertex_numb : max_degrees,
        min_deg = (max_deg > min_degrees) ? min_degrees : max_deg,
        start = 0;

    for(long long i = 0; i < vertex_numb; i++){
        degrees[i] = start;
        start += rand() % (max_deg - min_deg + 1) + min_deg;
    }
    degrees[vertex_numb] = start;

    long long *G = (long long*) malloc(sizeof(long long) * start),
        *options = (long long*) malloc(sizeof(long long) * vertex_numb);

    for(long long i = 0; i < vertex_numb; i++){
        reset_options(options, degrees[i + 1] - degrees[i]);
        for(long long j = degrees[i]; j < degrees[i + 1]; j++){
            G[j] = get_unique_random(options, degrees[i + 1] - j);
        }
    }

    *graph = G;

    free(options);
}

void print_graph(long long* G, long long* degrees, long long vertex_numb)
{
    for(long long i = 0; i < vertex_numb; i++)
    {
        printf("%d | ", i);
        for(long long j = degrees[i]; j < degrees[i + 1]; j++)
            printf("%d ", G[j]);
        printf("|\n");
    }
}

void print_distance(long long*d, long long vertex_numb, long long start)
{
    printf("distance(%d) = |\t ", start);
    for(long long i = 0; i < vertex_numb; i++)
        printf("%d\t ", d[i]);
    printf("|\n");
}

double calc(int argc, char** argv){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    long long vertex_numb = 1000000,
    max_degree = 10,
    min_degree = 1,
    start_node = 0;

    switch(argc){
        case(5): start_node = atoll(argv[4]);
        case(4): min_degree = atoll(argv[3]);
        case(3): max_degree = atoll(argv[2]);
        case(2): vertex_numb = atoll(argv[1]);
    }

    double dt;

    long long * G = NULL,
        * degrees = (long long*) malloc(sizeof(long long) * (vertex_numb + 1)),
        * d_d = (long long*) malloc(sizeof(long long) * vertex_numb);

    if(rank == MASTER){
        generate_random_graph(&G, degrees, vertex_numb, max_degree, min_degree);
        dt = MPI_Wtime();
    }

    bfs_dist(G, degrees, vertex_numb, start_node, d_d);

    if(rank == MASTER)
    {
        dt = MPI_Wtime() - dt;
        long long* d_s = (long long*) malloc(sizeof(long long) * vertex_numb);

        double st = MPI_Wtime();
        bfs_seq(G, degrees, vertex_numb,start_node, d_s);
        st = MPI_Wtime() - st;

        free(d_s);
    }

    free(d_d);
    free(degrees);
    free(G);

    return dt;
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double dt = 0;
    for(int i = 0; i < 10; i++){
        dt += calc(argc, argv);
    }

    if(rank == 0){
        printf("Avg is: %f\n", dt / 10.0);
    }

    MPI_Finalize();

    return 0;
}