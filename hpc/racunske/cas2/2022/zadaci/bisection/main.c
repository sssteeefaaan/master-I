#include <stdio.h>
#include <math.h>
#include <omp.h>

#define MAX_ITERS 1000
#define TOLERANCE 0.0000000001
#define STEP 1


#ifdef FX
    static double lower_bound = -5.f;
    static double upper_bound = 5.f;
#elif FPOW
    static double lower_bound = 15.f;
    static double upper_bound = 19.f;
#else
    static double lower_bound = -4.f;
    static double upper_bound = 10.f;
#endif

/**
 * Funkcija za koju treba pronaci korene.
 *
 * @param x - Vrednost nezavisne promenljive x.
 * @return Vrednost funkcije za zadatu vrednost promenljive x.
 */
double f(double x) {
#ifdef FX   // jedan koren nad intervalom
    return x - 1;
#elif FPOW  // nema korena nad intervalom
    return pow(x, 2) - 2;
#else //FSIN  // vise korena nad intervalom
    return sin(x);
#endif
}


/**
 * Funkcija koja metodom bisekcije racuna koren zadate funkcije. Pretpostavka je da je funkcija
 * nad zadatim intervalom monotona, odnosno da nema prevoja, jer metoda vraca samo prvi koren
 * funkcije koji pronadje.
 *
 * @param f         Funkcija ciji se koren trazi.
 * @param a         Pocetak intervala nad kojim se trazi koren funkcije. Podrazumeva se da vazi a < b.
 * @param b         Kraj intervala nad kojim se trazi koren funkcije. Podrazumeva se da vazi b > a.
 * @param xroot     Pokazivac na promenljivu u kojoj ce se nalaziti koren funkcije. Ukoliko funkcija
 *                  nad zadatim intervalom nema korena, vrednost ove promenljive je nepoznata.
 * @param maxiters  Maksimalan broj bisekcija koji ce biti izvrsen pre nego se odustane od trazenja
 *                  korena nad zadatim intervalom.
 * @param tol       Preciznost do na koju se trazi resenje. Veca preciznost dovodi do preciznijeg resenja,
 *                  ali povecava broj iteracija.
 */
double bisection(double (*f)(double), double a, double b, double *xroot, int maxiters, double tol) {
    
    double xp, fxp;  // pocetna tacka intervala u kom se trazi koren, vrednost funkcije u tacki
    double xk, fxk;  // krajnja tacka intervala u kom se trazi koren, vrednost funkcije u tacki
    double xmid;     // sredisnja tacka intervala u kom se trazi koren (xp - xk) / 2
    double fxmid;    // f(xmid)
    double dx;       // duzina intervala
    
    if (f(a) * f(b) > 0) return -1;     // nema korena u zadatom intervalu
    xp = a; xk = b;
    for (int i = 0; i < maxiters; i++) {
        dx = fabs(xk - xp) / 2;
        xmid = xp + dx;
        fxmid = f(xmid);
    
        if (fxmid == 0 || dx < tol) {
            *xroot = xmid;
            return 0;
        }
        
        if (f(xp) * fxmid < 0) {        // koren u podintervalu [xp, xmid]
            xk = xmid;
        } else if (f(xk) * fxmid < 0) { // koren u podintervalu [xmid, xk]
            xp = xmid;
        }
        
        //printf("--- Iteracija %d, xp = %lf, xk = %lf, xmid=%lf\n", i, xp, xk, xmid);
    }
    
    return 0;
}

/**
 * Funkcija za pronalazenje vise korena zadate funkcije f nad intervalom [a, b].
 *
 * Funkcija deli zadati interval na podintervale ne vece od zadatog koraka (videti step parametar).
 * Nad svakim dobijenim podintervalom [xp, xk] se metodom bisekcije pronalazi koren funkcije.
 * Ukoliko nad podintervalom [xp, xk] postoji vise korena funkcije, funkcija bisection ce vratiti
 * samo jedan. U slucaju da trenutna vrednost parametra sa trenutnom vrednoscu parametra step
 * ne mogu biti pronadjeni svi koreni funkcije, potrebno je smanjiti vrednost ovog parametra i
 * pokusati opet.
 *
 * @param f         Funkcija za koju se traze svi koreni nad intervalom [a, b].
 * @param a         Pocetak intervala nad kojim se traze koreni funkcije. Podrazumeva se da vazi a < b.
 * @param b         Kraj intervala nad kojim se traze koreni funkcije. Podrazumeva se da vazi b > a.
 * @param xroot     Pokazivac na promenljivu u kojoj ce se nalaziti koren funkcije. Ukoliko funkcija
 *                  nad zadatim intervalom nema korena, vrednost ove promenljive je nepoznata.
 * @param maxiters  Maksimalan broj bisekcija koji ce biti izvrsen pre nego se odustane od trazenja
 *                  korena nad zadatim intervalom.
 * @param tol       Preciznost do na koju se trazi resenje. Veca preciznost dovodi do preciznijeg resenja,
 *                  ali povecava broj iteracija.
 * @param step      Velicina podintervala [xp, xk] intervala [a, b] nad kojim ce biti trazeni koreni.
 */
void find_roots(double (*f)(double), double a, double b, double *xroot, int maxiters, double tol, double step) {
    double xp = a, xk = b;
    double dx = fabs(xk - xp) / 2;
    double xroot1, xroot2;
    
    if (fabs(xk - xp) < step) {
        if (bisection(f, xp, xk, xroot, maxiters, tol)) {
            printf("Interval [%lf, %lf]: Nema korena.\n", xp, xk);
        } else {
            printf("Interval [%lf, %lf]: Ima koren u %lf.\n", xp, xk, *xroot);
        }
    } else {
        find_roots(f, xp, xp + dx, &xroot1, maxiters, tol, step);
        find_roots(f, xp + dx, xk, &xroot2, maxiters, tol, step);
    }
}

int main() {
    
    double start = omp_get_wtime();
    
    double xroot, lb = lower_bound, ub = upper_bound;
    find_roots(f, lb, ub, &xroot, MAX_ITERS, TOLERANCE, STEP);

    double end = omp_get_wtime();
    printf("Ukupno trajanje: %lf\n", end - start);
    
    return 0;
}