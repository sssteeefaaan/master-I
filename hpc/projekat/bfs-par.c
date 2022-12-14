#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

const int MASTER = 0;
const int INFINITY = 0xffff;

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

void bfs_par(int* graph, int vertex_numb, int degree, int start, int* distance, int num_threads)
{
    int F_global_count = 0,
        work_load = vertex_numb / num_threads + 1;

    int *N = (int*) malloc(sizeof(int) * num_threads * num_threads * vertex_numb),
        *N_size = (int*) malloc(sizeof(int) * num_threads * num_threads);

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_num = omp_get_thread_num();

        #pragma omp for
        for(int i = 0; i < vertex_numb; i++)
            distance[i] = INFINITY;

        #pragma omp single
            distance[start] = 0;

        int* F = (int*) malloc(sizeof(int) * vertex_numb),
            F_count,
            level = 0;

        while(1){

            F_count = 0;

            for(int i = 0; i < work_load; i++){
                int index = thread_num * work_load + i;
                if(index < vertex_numb && distance[index] == level){
                    F[F_count++] = index;
                }
            }

            #pragma omp single
                F_global_count = 0;

            #pragma omp critical
                F_global_count += F_count;

            #pragma omp barrier

            if(F_global_count == 0)
                break;

            #pragma omp for
            for(int i = 0; i < num_threads * num_threads; i++)
                N_size[i] = 0;

            for(int i = 0; i < F_count; i++)
            {
                int node = F[i];
                for(int j = 0; j < degree; j++){
                    int neighbour = graph[node * degree + j],
                        thread_dest = neighbour / work_load,
                        offset = thread_dest * num_threads + thread_num,
                        k = 0;
                    while(k < N_size[offset] && N[offset * vertex_numb + k] != neighbour) k++;
                    if(k == N_size[offset]) {
                        N[offset * vertex_numb + (N_size[offset]++)] = neighbour;
                    }
                }
            }

            #pragma omp barrier

            for(int i = 0; i < num_threads; i++)
            {
                int offset = thread_num * num_threads + i;
                for(int j = 0; j < N_size[offset]; j++)
                    if(distance[N[offset * vertex_numb + j]] == INFINITY){
                        distance[N[offset * vertex_numb + j]] = level + 1;
                    }
            }

            #pragma omp barrier

            level++;
        }

        free(F);
    }

    free(N_size);
    free(N);
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
        * d_p = (int*) malloc(sizeof(int) * vertex_numb),
        * d_s = (int*) malloc(sizeof(int) * vertex_numb);

    printf("---------------------- GENERATING RANDOM GRAPH ----------------------\n\n");
    printf("vertex_numb = %d\ndegree = %d\nstart_node = %d\n\n", vertex_numb, degree, start_node);
    generate_random_graph(G, vertex_numb, degree);
    printf("---------------------- RANDOM GRAPH GENERATED ----------------------\n\n");

    printf("---------------------- PARALLEL EXECUTION ----------------------\n");
    dt = omp_get_wtime();
    bfs_par(G, vertex_numb, degree, start_node, d_p, omp_get_num_procs());
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