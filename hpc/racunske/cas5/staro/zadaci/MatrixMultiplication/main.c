#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>
#include <omp.h>

#ifndef DISABLE_HDF5
#include "utils/h5_matrix_utils.h"
#endif

#define M1FILENAME "../input_data/m3x3.h5"   // modifikovati putanju po potrebi
#define M2FILENAME "../input_data/m3x3.h5"   // modifikovati putanju po potrebi

void matrix_multiply_openmp(float *matrix1, float *matrix2, float *result) {
    // TODO implementirati OpenMP varijantu mnozenja matrica
}

void matrix_multiply_mpi(int *argc, char ***argv, float *matrix1,
                         float *matrix2, float *result) {
    // TODO implementirati OpenMPI varijantu mnozenja matrica
}

void matrix_multiply_hybrid(int *argc, char ***argv, float *matrix1,
                            float *matrix2, float *result) {
    // TODO Implementirati hibridnu OpenMP-OpenMPI varijantnu mnozenja matrica
}

int main(int argc, char *argv[]) {
    
    float *matrix1 = NULL, *matrix2 = NULL, *result = NULL;
    unsigned long long rows, cols;
    
#ifndef DISABLE_HDF5
    matrix1 = h5_load_matrix(M1FILENAME, &rows, &cols);
    assert(matrix1 != NULL);
#else
    printf("HDF podrska onemogucena!\n");
#endif

#ifndef DISABLE_HDF5
    matrix2 = h5_load_matrix(M2FILENAME, &rows, &cols);
    assert(matrix2 != NULL);
#else
    printf("HDF podrska onemogucena!\n");
#endif
    
    /**************************************************************************/
    /******************************** NAPOMENE ********************************/
    /**************************************************************************/
    /*
     Dat je primer cuvanja matrice u hdf5 formatu (upotrebiti za cuvanje rezultata)
     Pri odbrani ce biti koriscen alat h5diff za poredjenje rezultata
     prethodno generisane sekvencijalne verzije mnozenja matrica i rezultata
     dobijenog iz hibridnog resenja (manje rezlike u rezultatima na odredjenim
     decimalama su moguce usled aritmetike razlomljenih brojeva i nece se
     tretirati kao greska.
     
     Izgenerisati rezultate za sve date ulazne matrice (od 3x3 do 10240x10240)
     i smestiti ih u isti direktorijum direktorijum. Zadrzati konvenciju
     davanja imena datotekama rezultata.*/
    /*************************************************************************/
    matrix_multiply_hybrid(&argc, &argv, matrix1, matrix2, result);
    assert(result != NULL);
    
    char filename[15];
    sprintf(filename, "result%dx%d", rows, cols); // Pretpostavka je da ime datoteke nikada
                                                  // nece biti duze od 14 karaktera. Po potrebi promeniti.
    h5_save_float_matrix(filename, matrix1, rows, cols);
    
    if (matrix1 != NULL) free(matrix1);
    if (matrix2 != NULL) free(matrix2);
    if (result  != NULL) free(result);
    
    return 0;
}