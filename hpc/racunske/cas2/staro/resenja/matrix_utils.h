#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LL long long

void initialize_vector_random(float** v, LL N)
{
    *v = (float*) malloc(sizeof(float) * N);
    for(LL i = 0; i < N; i++)
        (*v)[i] = (random() % N) * 3.14f;
}

void initialize_matrix_random(float*** m, LL N, LL M)
{
    *m = (float**) malloc(sizeof(float*) * N);
    for(LL i = 0; i < N; i++)
        initialize_vector_random(&(*m)[i], M);
}

void initialize_vector_empty(float** v, LL N)
{
    *v = (float*) malloc(sizeof(float) * N);
}

void initialize_matrix_empty(float*** m, LL N, LL M)
{
    *m = (float**) malloc(sizeof(float*) * N);
    for(LL i = 0; i < N; i++)
        initialize_vector_empty(&(*m)[i], M);
}

void print_vector(float* v, LL N)
{
    printf("| ");
    for(LL i = 0; i < N; i++)
        printf("%.3f ", v[i]);
    printf("|\n");
}

void print_vector_label(char* label, float* v, LL N)
{
    printf("%s = ", label);
    print_vector(v, N);
    printf("\n");
}

void free_vector(float** v)
{
    free(*v);
    *v = NULL;
}

void free_matrix(float ***matrix, LL N)
{
    for(LL i = 0; i < N; i++)
        free_vector(&(*matrix)[i]);
    free(*matrix);
    *matrix = NULL; 
}

void multiply(float** matrix, LL N, LL M, float* vector, float* result)
{
    #pragma omp for
    for(LL i = 0; i < N; i++){
        result[i] = 0;
        for(LL j = 0; j < M; j++)
            result[i] += matrix[i][j] * vector[j];
    }
}

void print_matrix(float** m, LL N, LL M){
    for(LL i = 0; i < N; i++)
        print_vector(m[i], M);
    printf("\n");
}

void print_matrix_label(char* label, float** m, LL N, LL M){
    printf("%s:\n", label);
    print_matrix(m, N, M);
    printf("\n");
}

void copy_vector(float** result, const float* original, LL N){
    *result = (float*) malloc(sizeof(float) * N);
    for(LL i = 0; i < N; i++)
        (*result)[i] = original[i];
}

void copy_matrix(float*** result, const float** original, LL N, LL M){
    *result = (float**) malloc(sizeof(float*) * N);
    for(LL i = 0; i < N; i++)
        copy_vector(&((*result)[i]), original[i], M);
}

short compare_vectors(const float *v1, const float* v2, LL N)
{
    short test = 0;
    for(LL i = 0; !test && i < N; i++)
        test = v1[i] - v2[i];
    return test;
}

short compare_matrices(const float **m1, const float** m2, LL N, LL M)
{
    short test = 0;
    for(LL i = 0; !test && i < N; i++)
        test = compare_vectors(m1[i], m2[i], M);
    return test;
}

void multiply_matrices(const float** mat1, const float** mat2, float** result, LL N, LL M, LL K)
{
    #pragma omp for
    for(LL i = 0; i < N; i++)
    {
        for(LL j = 0; j < K; j++)
        {
            result[i][j]=0;
            for(LL k = 0; k < M; k++)
                result[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}

void
transpose_matrix
(float** matrix_like_vector, LL N, LL M)
{
    float* v = *matrix_like_vector;
    *matrix_like_vector = (float*) malloc(sizeof(float) * N * M);
    #pragma omp parallel for
    for(LL i = 0; i < N; i++)
        for(LL j = 0; j < M; j++)
            (*matrix_like_vector)[j * N + i] = v[i * M + j];
    free(v);
}