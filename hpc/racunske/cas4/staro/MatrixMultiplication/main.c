#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef DISABLE_HDF5
#include "utils/h5_matrix_utils.h"
#endif

#define M1FILENAME "../input_data/m3x3.h5"   // modifikovati putanju po potrebi
#define M2FILENAME "../input_data/m3x3.h5"   // modifikovati putanju po potrebi

// TODO implementirati sekvencijalnu varijantu mnozenja matrice i vektora.
// Dodati potrebne parametre i u main-u meriti vreme izvrsavanja.
// Za merenje vremena koristiti funkciju MPI_Wtime().
void matrix_multiply();

// TODO implementirati OpenMPI varijantu mnozenja matrice i vektora.
// Dodati potrebne parametre i u main-u meriti vreme izvrsavanja.
// U main-u ispisati oba vremena izvrsavanja.
// Za merenje vremena koristiti funkciju MPI_Wtime().
void matrix_multiply_mpi();

int main() {
    
    float *matrix1 = NULL, *matrix2 = NULL, *result = NULL;
    unsigned long long rows, cols;
    
#ifndef DISABLE_HDF5
    printf("Matrica 1:\n");
    matrix1 = h5_load_matrix(M1FILENAME, &rows, &cols);
    print_float_matrix(matrix1, rows, cols);
    assert(matrix1 != NULL);
#else
    printf("HDF podrska onemogucena!\n");
#endif

#ifndef DISABLE_HDF5
    printf("Matrica 2:\n");
    matrix2 = h5_load_matrix(M2FILENAME, &rows, &cols);
    print_float_matrix(matrix2, rows, cols);
    assert(matrix2 != NULL);
#else
    printf("HDF podrska onemogucena!\n");
#endif
    
    // Primer ispisa matrice (upotrebiti za ispis rezultata)
    // Pri odbrani zadatka ispis rezultata se gleda samo za male matrice
    // (3x3 i 5x5).
    printf("\nTODO ovde ispisati rezultat\n");
    print_float_matrix(matrix1, rows, cols);
    
    
    if (matrix1 != NULL) free(matrix1);
    if (matrix2 != NULL) free(matrix2);
    
    return 0;
}