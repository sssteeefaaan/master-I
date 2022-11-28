#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void sequential_function(int limit, int step, int rand_operand) {
    int max_y = 0, x = 0, best_x = 0, y;

    while (x <= limit) {
        y = x + x / 2 + rand_operand + x * 2 - 4 + x % 6 - x / 3;
        // printf("X: %d. Y: %d\n", x, y);

        if (y > max_y) {
            best_x = x;
            max_y = y;
        }

        x += step;
        x = x * -1;
        step = step * -1;
    }
    printf("Best x: %d. Max y: %d\n", best_x, max_y);
}

void do_work(int i, int step, int rand_operand, int limit, int* best_x, int* max_y)
{
    int x = step * pow(-1, i) * i;
    if (x <= limit){
        #pragma omp task
        do_work(i + 1, step, rand_operand, limit, best_x, max_y);
        int y = x + x / 2 + rand_operand + x * 2 - 4 + x % 6 - x / 3;
        // printf("X: %d. Y: %d\n", x, y);

        #pragma omp critical
        if (y > *max_y) {
            *best_x = x;
            *max_y = y;
        }
    }
}

void parallel_function(int limit, int step, int rand_operand) {
    int max_y = 0, best_x = 0;

    //TODO: implementirati paralelni kod
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            do_work(0, step, rand_operand, limit, &best_x, &max_y);
        }
    }
    printf("Best x: %d. Max y: %d\n", best_x, max_y);
}

void parallel_function2(int limit, int step, int rand_operand) {
    int max_y = 0, best_x = 0;

    int n = limit / step + 1;
    if (n % 2){
        n += 1;
    }
    #pragma omp parallel for
    for(int i = 0; i < n; i++)
    {
        int x = i * step * pow(-1, i);
        int y = x + x / 2 + rand_operand + x * 2 - 4 + x % 6 - x / 3;
        // printf("X: %d. Y: %d\n", x, y);

        #pragma omp critical
        if (y > max_y) {
            best_x = x;
            max_y = y;
        }
    }
    printf("Best x: %d. Max y: %d\n", best_x, max_y);
}

int main(int argc, char* argv[]) {
    
    time_t randVreme = time(0);
    srand(randVreme);

    int x = 0, step = rand() % 10 + 1;
    int y, max_y = 0;
    int best_x = x;

    int limit = rand() % 3467 + 112;
    // limit = 230;
    int rand_operand = rand() % 100 + 1;

    int dt;
    printf("--------------------- SEQUENTIAL CODE ---------------------\n");
    dt = omp_get_wtime();
    sequential_function(limit, step, rand_operand);
    dt = omp_get_wtime() - dt;
    printf("Time taken: %f\n", dt);
    printf("-----------------------------------------------------------\n\n");

    printf("--------------------- PARALLEL CODE ---------------------\n");
    dt = omp_get_wtime();
    parallel_function(limit, step, rand_operand);
    dt = omp_get_wtime() - dt;
    printf("Time taken: %f\n", dt);
    printf("-----------------------------------------------------------\n\n");

    printf("--------------------- PARALLEL CODE 2 ---------------------\n");
    dt = omp_get_wtime();
    parallel_function2(limit, step, rand_operand);
    dt = omp_get_wtime() - dt;
    printf("Time taken: %f\n", dt);
    printf("-----------------------------------------------------------\n\n");
}