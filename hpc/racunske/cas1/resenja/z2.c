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
    double r;
    double i;
};

void testpoint(struct d_complex);
struct d_complex c;
int numoutside = 0;

int main(){
    int i, j;
    double area, error, eps = 1.0e-5;
    #pragma omp parallel for private(eps, i, j) reduction(+ : numoutside)
    for (i = 0; i < NPOINTS; i++) {
        struct d_complex z;
        for (j = 0; j < NPOINTS; j++) {
            z.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + eps;
            z.i = 1.125 * (double)(j) / (double)(NPOINTS) + eps;
            testpoint(z);
        }
    }
    area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS*NPOINTS-numoutside) / (double)(NPOINTS*NPOINTS);
    error = area / (double)NPOINTS;

    printf("area = %lf, error = %lf\n", area, error);

    return 0;
}

void testpoint(struct d_complex z){
    int iter;
    double temp;
    for (iter = 0; iter < MXITR; iter++){
        temp = (z.r * z.r) - (z.i * z.i) + c.r;
        z.i = z.r * z.i * 2 + c.i;
        z.r = temp;
        if ((z.r * z.r + z.i * z.i) > 4.0) {
            numoutside++;
            break;
        }
    }
}