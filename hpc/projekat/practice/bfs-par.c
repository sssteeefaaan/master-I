#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const long long MASTER = 0;
const long long INFINITY = 0xffff;

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

void bfs_seq(long long* graph, long long vertex_numb, long long degree, long long start, long long* distance)
{
    for(long long i = 0; i < vertex_numb; i++)
        distance[i] = INFINITY;
    distance[start] = 0;

    long long *fs = (long long*) malloc(sizeof(long long) * vertex_numb),
        *ns = (long long*) malloc(sizeof(long long) * vertex_numb);

    fs[0] = start;

    long long level = 1,
        count1 = 1,
        count2 = 0;

    while(count1 > 0)
    {
        for(long long i = 0; i < count1; i++){
            long long node = fs[i];
            for(long long j = 0; j < degree; j++){
                long long neighbour = graph[node * degree + j];
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

void bfs_par(long long* graph, long long vertex_numb, long long degree, long long start, long long* distance, long long num_threads)
{
    long long F_global_count = 0,
        work_load = vertex_numb / num_threads + 1;

    long long *N = (long long*) malloc(sizeof(long long) * num_threads * num_threads * vertex_numb),
        *N_size = (long long*) malloc(sizeof(long long) * num_threads * num_threads);

    #pragma omp parallel num_threads(num_threads)
    {
        long long thread_num = omp_get_thread_num();

        #pragma omp for
        for(long long i = 0; i < vertex_numb; i++)
            distance[i] = INFINITY;

        #pragma omp single
            distance[start] = 0;

        long long* F = (long long*) malloc(sizeof(long long) * vertex_numb),
                * dups = (long long*) malloc(sizeof(long long) * num_threads * vertex_numb),
                F_count,
                level = 0;

        for(long long i = 0; i < num_threads * vertex_numb; dups[i++] = 0);

        while(1){

            F_count = 0;

            for(long long i = 0; i < work_load; i++){
                long long index = thread_num * work_load + i;
                if(index < vertex_numb && distance[index] == level){
                    F[F_count++] = index;
                }
            }

            #pragma omp single
                F_global_count = 0;

            #pragma omp critical
                F_global_count |= F_count;

            #pragma omp barrier

            if(F_global_count == 0)
                break;

            #pragma omp for
            for(long long i = 0; i < num_threads * num_threads; i++)
                N_size[i] = 0;

            for(long long i = 0; i < F_count; i++)
            {
                long long node = F[i];
                for(long long j = 0; j < degree; j++){
                    long long neighbour = graph[node * degree + j],
                        thread_dest = neighbour / work_load,
                        offset = thread_dest * num_threads + thread_num;
                    if(dups[thread_dest * num_threads + neighbour] == 0){
                        N[offset * vertex_numb + (N_size[offset]++)] = neighbour;
                        dups[thread_dest * num_threads + neighbour] = 1;
                    }
                }
            }

            #pragma omp barrier

            for(long long i = 0; i < num_threads; i++)
            {
                long long offset = thread_num * num_threads + i;
                for(long long j = 0; j < N_size[offset]; j++)
                    if(distance[N[offset * vertex_numb + j]] == INFINITY){
                        distance[N[offset * vertex_numb + j]] = level + 1;
                    }
            }

            level++;
        }

        free(F);
    }

    free(N_size);
    free(N);
}

void generate_random_graph(long long* graph, long long vertex_numb, long long degree)
{
    srand(time(NULL));
    for(long long i = 0; i < vertex_numb; i++){
        for(long long j = 0; j < degree; j++){
            long long val;
            do{
                val = rand() % vertex_numb;
            }while(val == i);

            long long k = 0;
            while(val != graph[i * degree + k] && k < degree) k++;
            if(k == degree)
                graph[i * degree + j] = val;
            else j--;
        }
    }
}

void print_graph(long long*G, long long vertex_numb, long long degree)
{
    for(long long i = 0; i < vertex_numb; i++)
    {
        printf("%d | ", i);
        for(long long j = 0; j < degree; j++)
            printf("%d ", G[i * degree + j]);
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

long long main(long long argc, char** argv)
{
    long long vertex_numb = 1000000,
    degree = 1,
    start_node = 0,
    num_threads = omp_get_num_procs();

    switch(argc){
        case (5): num_threads = atoi(argv[4]);
        case(4): start_node = atoi(argv[3]);
        case(3): degree = atoi(argv[2]);
        case(2): vertex_numb = atoi(argv[1]);
    }

    double dt;

    long long * G = (long long*)malloc(sizeof(long long) * vertex_numb * degree),
        * d_p = (long long*) malloc(sizeof(long long) * vertex_numb),
        * d_s = (long long*) malloc(sizeof(long long) * vertex_numb);

    printf("---------------------- GENERATING RANDOM GRAPH ----------------------\n\n");
    printf("vertex_numb = %d\ndegree = %d\nstart_node = %d\n\n", vertex_numb, degree, start_node);
    generate_random_graph(G, vertex_numb, degree);
    printf("---------------------- RANDOM GRAPH GENERATED ----------------------\n\n");

    printf("---------------------- PARALLEL EXECUTION ----------------------\n");
    dt = omp_get_wtime();
    bfs_par(G, vertex_numb, degree, start_node, d_p, num_threads);
    dt = omp_get_wtime() - dt;
    printf("---------------------- Time taken %f ------------------------\n", dt);
    printf("---------------------- PARALLEL COMPLETE ----------------------\n\n");

    printf("---------------------- SEQUENTIAL EXECUTION ----------------------\n");
    dt = omp_get_wtime();
    bfs_seq(G, vertex_numb, degree, start_node, d_s);
    dt = omp_get_wtime() - dt;
    printf("---------------------- Time taken %f --------------------------\n", dt);
    printf("---------------------- SEQUENTIAL COMPLETE ----------------------\n\n");

    printf("---------------------- VALIDATING RESULTS ----------------------\n");
    compare_vectors(d_p, d_s, vertex_numb) ? printf("Correct!\n") : printf("False!\n");
    printf("---------------------- VALIDATION COMPLETE ----------------------\n");

    free(d_s);
    free(d_p);
    free(G);

    return 0;
}