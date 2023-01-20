#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int MASTER = 0;
const int INF = 0xffff;

void print_vector(char* label, int*v, int n)
{
    printf("%s\t |", label);
    for(int i = 0; i < n; i++)
        printf("\t %d", v[i]);
    printf("\t|\n");
}

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

void bfs_seq(
    char* graph,
    int vertex_numb,
    int start,
    int* distance
){
    for(int i = 0; i < vertex_numb; i++)
        distance[i] = INF;
    distance[start] = 0;

    int *fs = (int*) malloc(sizeof(int) * vertex_numb),
        *ns = (int*) malloc(sizeof(int) * vertex_numb);

    fs[0] = start;

    int level = 0,
        count1 = 1,
        count2 = 0;

    while(count1 > 0)
    {
        for(int i = 0; i < count1; i++){
            int node = fs[i];
            for(int j = 0; j < vertex_numb; j++){
                if(graph[node * vertex_numb + j] != 0 && distance[j] == INF){
                    distance[j] = level + 1;
                    ns[count2++] = j;
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

void print_submatrix(int rank, int col_rank, int row_rank, char* mat, int n){
    printf("P[%d](%d, %d)\n", rank, col_rank, row_rank);
    for(int i = 0; i < n; i++)
    {
        printf("|\t");
        for(int j = 0; j < n; j++){
            printf("%d\t ", mat[i * n + j]);
        }
        printf("|\n");
    }
}

void bfs_dist(
    char* graph,
    int vertex_numb,
    int start,
    int *distance
){
    int rank, size, q;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    q = sqrt(size);

    int work_load = vertex_numb / q;

    MPI_Comm row, col;
    MPI_Comm_split(MPI_COMM_WORLD, rank % q, rank, &col);
    MPI_Comm_split(MPI_COMM_WORLD, rank / q, rank, &row);

    int col_rank, row_rank;
    MPI_Comm_rank(col, &col_rank);
    MPI_Comm_rank(row, &row_rank);

    char* local_graph = (char*) malloc(sizeof(char) * work_load * work_load);
    MPI_Datatype sub_matrix;
    int sizes[2] = {vertex_numb, vertex_numb};
    int subsizes[2] = {work_load, work_load};
    int starts[2] = {col_rank * work_load * vertex_numb, row_rank * work_load * vertex_numb};
    MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_CHAR, &sub_matrix);
    MPI_Type_commit(&sub_matrix);
    MPI_Type_create_resized(sub_matrix, 0, sizeof(char), &sub_matrix);
    MPI_Type_commit(&sub_matrix);

    int* sendcounts = (int*) malloc(sizeof(int) * size);
    int* displacements = (int*) malloc(sizeof(int) * size);
    for(int i = 0; i < size; i++){
        sendcounts[i] = 1;
        displacements[i] = work_load * (i % q) + vertex_numb * (i / q) * work_load;
    }

    MPI_Scatterv(graph, sendcounts, displacements, sub_matrix, local_graph, work_load * work_load, MPI_CHAR, MASTER, MPI_COMM_WORLD);

    free(displacements);
    free(sendcounts);

    int *d = (int*) malloc(sizeof(int) * work_load);
    for(int i = 0; i < work_load; i++)
        d[i] = INF;
    if(start / work_load == row_rank)
        d[start] = 0;

    int *F = (int*) malloc(sizeof(int) * vertex_numb),
        *locals = (int*) malloc(sizeof(int) * vertex_numb),
        *N = (int*) malloc(sizeof(int) * vertex_numb);

    int level = 0,
        F_count,
        F_col_count,
        F_global_count;

    while(1) {

        F_count = F_col_count = F_global_count = 0;

        for(int  i = 0; i < work_load; i++){
            locals[i] = 0;
            if(d[i] == level){
                locals[i] = 1;
                F_count++;
            }
        }

        MPI_Allreduce(&F_count, &F_col_count, 1, MPI_INT, MPI_SUM, col);

        MPI_Allreduce(&F_col_count, &F_global_count, 1, MPI_INT, MPI_SUM, row);

        if(F_global_count == 0)
            break;

        MPI_Allgather(locals, work_load, MPI_INT, F, work_load, MPI_INT, col);

        for(int i = 0; i < vertex_numb; i++) locals[i] = 0;

        for(int i = 0; i < work_load; i++){
            if(F[col_rank * work_load + i] == 1){
                for(int j = 0; j < work_load; j++){
                    if(local_graph[i * work_load + j] == 1)
                        locals[row_rank * work_load + j] = 1;
                }
            }
        }

        MPI_Allreduce(locals, N, vertex_numb, MPI_INT, MPI_MAX, row);

        for(int j = 0; j < work_load; j++)
            if(N[col_rank * work_load + j] == 1 && d[j] == INF)
                d[j] = level + 1;

        level++;
    }

    MPI_Gather(d, work_load, MPI_INT, distance, work_load, MPI_INT, MASTER, row);

    free(N);
    free(locals);
    free(F);
    free(d);
    free(local_graph);
}

void reset_options(int *options, int size){
    for(int i = 0; i < size; i++)
        options[i] = i;
}

int get_unique_random(int *options, int maximum){
    int cursor = rand() % maximum,
    tmp = options[cursor];
    options[cursor] = options[maximum - 1];
    return options[maximum - 1] = tmp;
}

void remove_option(int* options, int n, int r){
    for(int i = 0; i < n; i++)
        if(options[i] == r){
            options[i] = options[n - 1];
            break;
        }
}

void print_graph(char* G, int vertex_numb)
{
    for(int i = 0; i < vertex_numb; i++)
    {
        printf("%d | ", i);
        for(int j = 0; j < vertex_numb; j++)
            printf("%d ", G[i * vertex_numb + j]);
        printf("|\n");
    }
}

void generate_random_graph(
    char* graph,
    int vertex_numb,
    int max_degrees,
    int min_degrees
){
    int max_deg = vertex_numb < max_degrees ? vertex_numb : max_degrees,
        min_deg = (max_deg > min_degrees) ? min_degrees : max_deg;

    int *options = (int*) malloc(sizeof(int) * vertex_numb);
    int *degrees = (int*) malloc(sizeof(int) * vertex_numb);

    for(int i = 0; i < vertex_numb; i++) {
        degrees[i] = 0;
        for(int j = 0; j < vertex_numb; j++)
            graph[i * vertex_numb + j] = 0;
    }

    reset_options(options, vertex_numb);

    int n = vertex_numb;

    for(int i = 0; i < vertex_numb; i++){
        int rand_deg = rand() % (max_deg - min_deg + 1) + min_deg;
        for(int j = 0; degrees[i] < rand_deg && j < n; j++){
            int rand = get_unique_random(options, n - j);
            if(graph[i * vertex_numb + rand] != 0){
                for(int k = 0; k < n; k++){
                    if(degrees[options[k]] < max_deg && graph[i * vertex_numb + options[k]] == 0){
                        rand = options[k];
                        break;
                    }
                }
            }
            graph[i * vertex_numb + rand] =
                graph[rand * vertex_numb + i] = 1;
            degrees[i]++;
            if(degrees[i] == max_deg){
                remove_option(options, n, i);
                n--;
            }
            if(i != rand){
                degrees[rand]++;
                if(degrees[rand] == max_deg){
                    remove_option(options, n, rand);
                    n--;
                }
            }
        }
    }

    free(degrees);
    free(options);
}

int check_if_symmetric(char* g, int n){
    int flag = 1;
    for(int i = 0; flag && i < n; i++)
        for(int j = 0; flag && j <= i; j++)
            flag = g[i * n + j] == g[j * n + i];
    return flag;
}
int check_min(char* g, int n, int min){
    int my_deg = 0;
    for(int i = 0; my_deg <= min && i < n; i++){
        my_deg = 0;
        for(int j = 0; my_deg <= min && j < n; j++)
            if(g[i * n + j])
                my_deg++;
    }
    return my_deg >= min;
}
int check_max(char* g, int n, int max){
    int my_deg = max;
    for(int i = 0; my_deg > 0 && i < n; i++){
        my_deg = max;
        for(int j = 0; my_deg > 0 && j < n; j++)
            if(g[i * n + j] == 0)
                my_deg--;
    }
    return my_deg <= max;
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

    int vertex_numb = 10000,
    max_degree = 10,
    min_degree = 1,
    start_node = 0;

    switch(argc){
        case(5): start_node = atoi(argv[4]);
        case(4): min_degree = atoi(argv[3]);
        case(3): max_degree = atoi(argv[2]);
        case(2): vertex_numb = atoi(argv[1]);
    }

    char *G = (char*) malloc(sizeof(char) * vertex_numb * vertex_numb);
    int *d_d = (int*) malloc(sizeof(int) * vertex_numb),
    *d_s = NULL;

    double dt, st;

    if(rank == MASTER){
        printf("[Number of vertexes]: %d\n", vertex_numb);
        printf("[Maximum vertex degree]: %d\n", max_degree);
        printf("[Minimum vertex degree]: %d\n", min_degree);
        printf("[Starting node]: %d\n", start_node);
        printf("[Random graph generation]: Starting!\n");
        generate_random_graph(G, vertex_numb, max_degree, min_degree);
        printf("[Random graph generation]: Completed!\n");
        printf("[Symmetric graph]: %s!\n", check_if_symmetric(G, vertex_numb) ? "Correct" : "False");

        d_s = (int*) malloc(sizeof(int) * vertex_numb);
        st = MPI_Wtime();
        bfs_seq(G, vertex_numb, start_node, d_s);
        st = MPI_Wtime() - st;

        printf("[Sequential calculations]: Time taken (%f)\n", st);

        dt = MPI_Wtime();
    }

    bfs_dist(G, vertex_numb, start_node, d_d);

    if(rank == MASTER)
    {
        dt = MPI_Wtime() - dt;
        printf("[Distributed calculations]: Time taken (%f)\n", dt);

        printf("[Results]: %s\n", compare_vectors(d_d, d_s, vertex_numb) ? "Correct!" : "False!");
        printf("[Speedup]: %f\n", st/dt);

        print_distance(d_s, vertex_numb, start_node);
        print_distance(d_d, vertex_numb, start_node);

        free(d_s);
    }

    free(d_d);
    free(G);

    MPI_Finalize();

    return 0;
}