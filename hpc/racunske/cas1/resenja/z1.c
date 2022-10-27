#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv){
    long long n = 1 << 10;

    if (argc > 1)
        n = atoll(argv[1]);

    double step = 1.0 / n,
        pi = 0,
        x;
    #pragma omp parallel for reduction(+ : pi) private(x)
    for(long long i = 0; i < n; i++)
    {
        x = (i + .5) * step;
        pi += 4.0 / (1 +  x * x);
    }

    pi = step * pi;
    
    printf("pi = %f, my_pi = %f\n", pi, M_PI);

    return 0;
}