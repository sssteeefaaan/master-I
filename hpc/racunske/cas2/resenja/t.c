#include <stdio.h>
#include <omp.h>

int k = 0;

void f(){ printf("T[%d]: f()\n", omp_get_thread_num()); k += 5; }
void g(){ printf("T[%d]: g()\n", omp_get_thread_num()); k += 6; }

int main(){
    #pragma omp parallel
    {
        
        #pragma omp for
        for(int i = 0; i < 20; i++)
        {
                #pragma omp task
                {
                    i % 2 ? f() : g();
                    printf("T[%d]: k = %d\n", omp_get_thread_num(), k);
                }
        }
    }
    printf("k = %d\n", k);
    return 0;
}