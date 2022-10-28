/**
 * OpenMP paralelizovan Mandelbrotov algoritam. Program ne daje ispravne rezultate
 * zbog stetnog preplitanja koje se pojavljuje u njemu. Pronaci i otkloniti greske koje
 * su uzrok stetnom preplitanju. Ocekivani izlaz ispravnog programa za promenljivu area
 * je oko 1.512, dok je ocekivani izlaz za promenljivu error oko 0.0052.
 * 
 * Smernice:
 *  - Proveriti da li vrednosti promenljivih zadatih u zadatku utice na rezultat (npr.
 *  promena vrednosti promenljive eps).
 *  - Videti kako promena broja niti utice na tacnost rezultata. Pokrenuti program iz 
 *  terminala sa OMP_NUM_THREADS=1 && ./a.out. Izmeniti vrednosti zadate za broj niti
 *  i naziv izvrsne datoteke tako da odgovaraju zadatku.
 *  - Videti kako vise pokretanja iste izvrsne datoteke sa fiksiranim brojem niti utice
 *  na rezultat.
 */

#include <stdio.h>
#include <omp.h>

#define NPOINTS 1000
#define MXITR 1000

struct d_complex {
    double r; double i;
};
void testpoint(struct d_complex, int*);

int main(){
    int i, j;
    double area, error, eps = 1.0e-10;
    struct d_complex c;
    int numoutside = 0;

    //int num_procs = omp_get_num_procs();
    #pragma omp parallel default(shared) private(i, j, c, eps)
    //num_threads(num_procs)
    {
        #pragma omp master
            printf("Threads in a parallel region is %d.\n", omp_get_num_threads());

        #pragma omp for reduction(+ : numoutside)
        for (i = 0; i < NPOINTS; i++) {
            for (j = 0; j < NPOINTS; j++) {
                c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + eps;
                c.i = 1.125 * (double)(j) / (double)(NPOINTS) + eps;
                testpoint(c, &numoutside);
            }
        }
    }
    area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS*NPOINTS-numoutside) / (double)(NPOINTS*NPOINTS);
    error = area / (double)NPOINTS;

    printf("area = %lf, error = %lf\n", area, error);

    return 0;
}

void testpoint(struct d_complex c, int* numoutside){
    struct d_complex z;
    int iter;
    double temp;
    z = c;
    for (iter = 0; iter < MXITR; iter++){
        temp = (z.r * z.r) - (z.i * z.i) + c.r;
        z.i = z.r * z.i * 2 + c.i;
        z.r = temp;
        if ((z.r * z.r + z.i * z.i) > 4.0) {
            (*numoutside)++;
            break;
        }
    }
}