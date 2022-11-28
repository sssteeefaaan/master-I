#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define N 5
#define M 5

void compare_arrs(int* arr1, int* arr2) {
    int possible_missmatches[N * M];
    int num_mismatches = 0;
    for(int i = 0; i < N * M; i++) {
        if(arr1[i] != arr2[i]) {
            possible_missmatches[num_mismatches++] = i;
        }
    }

    printf("Number of mismatches: %d\n", num_mismatches);
    if(num_mismatches > 0) {
        printf("Mismatches on indexes: ");
        for(int i = 0; i < num_mismatches; i++) {
            printf("%d ", possible_missmatches[i]);
        }
    }
    printf("\n");
}

void print_arr(int* arr, char* arr_name) {
    printf("Array %s: { ", arr_name);
    for(int i = 0; i < N * M; i++) {
        printf("%d ", arr[i]);
    }
    printf("}\n\n");
}

int main(int argc, char* argv) {
    time_t randVreme = time(0);
    srand(randVreme);

    int arr1[M*N], test1[M*N], test2[M*N];

    for(int i = 0; i < M * N; i++) {
        arr1[i] = rand() % 6 + 1;
        test1[i] = arr1[i];
        test2[i] = arr1[i];
    } 

    for (int i = 1; i < N; i++) {
        for(int j = 0; j < M; j++) {
            test1[i * M + j] = test1[(i - 1) * M + j];
        }
    }

    #pragma omp parallel for
    for(int j = 0; j < M; j++) 
        for (int i = 1; i < N; i++)
                test2[i * M + j] = test2[(i - 1) * M + j];

    print_arr(arr1, "arr1");
    print_arr(test1, "arr2");
    print_arr(test2, "arr3");

    printf("Comparing test1 and test2...\n");
    compare_arrs(test1, test2);
    printf("\n");
}