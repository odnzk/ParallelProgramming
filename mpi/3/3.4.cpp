#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define MATRIX_SIZE 100

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    int ranks;
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (MATRIX_SIZE != ranks) {
        printf("Invalid conditions \n");
        MPI_Finalize();
        return 1;
    }

    int matrix[MATRIX_SIZE][MATRIX_SIZE];
    int helper[MATRIX_SIZE];
    if (rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrix[i][j] = i + j;
            }
        }
    }
    for (int i = 0; i < MATRIX_SIZE; i++) {
        MPI_Scatter(&matrix[i][rank], 1, MPI_INT,&helper[i], 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    int part = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        part += abs(helper[i]);
    }

    int global_norm;
    MPI_Reduce(&part, &global_norm, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printMatrix(matrix);
        printf("Norm: %d\n", global_norm);
    }

    MPI_Finalize();
    return 0;
}