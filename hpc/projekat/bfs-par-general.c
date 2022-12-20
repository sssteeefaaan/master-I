#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void bfs_seq(long long* graph, long long* degrees, long long vertex_numb, long long start, long long* distance)
{
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

void bfs_par(long long* graph, long long* degrees, long long vertex_numb, long long start, long long* distance, long long num_threads)
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
                for(long long j = degrees[node]; j < degrees[node + 1]; j++){
                    long long neighbour = graph[j],
                        thread_dest = neighbour / work_load,
                        offset = thread_dest * num_threads + thread_num;
                    if(dups[thread_dest * num_threads + neighbour] == 0){
                        N[offset * vertex_numb + N_size[offset]++] = neighbour;
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

void generate_random_graph(long long** graph, long long* degrees, long long vertex_numb, long long max_degrees, long long min_degrees)
{
    long long max_deg = (vertex_numb - 1) < max_degrees ? (vertex_numb - 1) : max_degrees,
        min_deg = (max_deg > min_degrees) ? min_degrees : max_deg,
        start = 0;

    for(long long i = 0; i < vertex_numb; i++){
        degrees[i] = start;
        start += rand() % (max_deg - min_deg + 1) + min_deg;
    }
    degrees[vertex_numb] = start;

    long long *G = (long long*) malloc(sizeof(long long) * start);

    for(long long i = 0; i < vertex_numb; i++){
        for(long long j = degrees[i]; j < degrees[i + 1]; j++){
            long long val, flag;

            do{
                val = rand() % vertex_numb;
                flag = 1;
                for(long long k = degrees[i]; flag && k < j; k++)
                    flag = G[k] != val;
            }while(flag == 0);

            G[j] = val;
        }
    }

    *graph = G;
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

int main(int argc, char** argv)
{
    long long
        num_threads = omp_get_num_procs(),
        vertex_numb = 1000000,
        max_degree = 10,
        min_degree = 1,
        start_node = 0;

    switch(argc){
        case(6): start_node = atoll(argv[5]);
        case(5): min_degree = atoll(argv[4]);
        case(4): max_degree = atoll(argv[3]);
        case(3): vertex_numb = atoll(argv[2]);
        case(2): num_threads = atoi(argv[1]);
    }

    double pt, st;

    long long * G = NULL,
        * degrees = (long long*) malloc(sizeof(long long) * (vertex_numb + 1)),
        * d_p = (long long*) malloc(sizeof(long long) * vertex_numb),
        * d_s = (long long*) malloc(sizeof(long long) * vertex_numb);

    printf("[Number of threads]: %d\n", num_threads);
    printf("[Number of vertexes]: %d\n", vertex_numb);
    printf("[Maximum vertex degree]: %d\n", max_degree);
    printf("[Minimum vertex degree]: %d\n", min_degree);
    printf("[Starting node]: %d\n", start_node);

    printf("[Random graph generation]: Starting!\n");
    generate_random_graph(&G, degrees, vertex_numb, max_degree, min_degree);
    printf("[Random graph generation]: Completed!\n");
    printf("[Graph size]: %d\n", degrees[vertex_numb]);

    pt = omp_get_wtime();
    bfs_par(G, degrees, vertex_numb, start_node, d_p, num_threads);
    pt = omp_get_wtime() - pt;
    printf("[Parallel calculations]: Time taken (%f)\n", pt);

    st = omp_get_wtime();
    bfs_seq(G, degrees,  vertex_numb, start_node, d_s);
    st = omp_get_wtime() - st;
    printf("[Sequential calculations]: Time taken (%f)\n", st);

    printf("[Results]: %s\n", compare_vectors(d_p, d_s, vertex_numb) ? "Correct!" : "False!");

    printf("[Speedup]: %f\n", st/pt);

    free(d_s);
    free(d_p);
    free(degrees);
    free(G);

    return 0;
}