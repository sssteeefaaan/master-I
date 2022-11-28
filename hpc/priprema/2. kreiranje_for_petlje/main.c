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
    printf("Best x: %d. Max y :%d\n", best_x, max_y);
}

void parallel_function(int limit, int step, int rand_operand) {
    int max_y = 0, best_x = 0, x = 0, y;

    //TODO: implementirati paralelni kod

    printf("Best x: %d. Max y :%d\n", best_x, max_y);
}

int main(int argc, char* argv[]) {
    
    time_t randVreme = time(0);
    srand(randVreme);

    int x = 0, step = 10;
    int y, max_y = 0;
    int best_x = x;

    int limit = rand() % 3467 + 112;
    // limit = 230;
    int rand_operand = rand() % 100 + 1;

    printf("--------------------- SEQUENTIAL CODE ---------------------\n");
    sequential_function(limit, step, rand_operand);
    printf("-----------------------------------------------------------\n\n");

    printf("--------------------- PARALLEL CODE ---------------------\n");
    parallel_function(limit, step, rand_operand);
    printf("-----------------------------------------------------------\n\n");
}