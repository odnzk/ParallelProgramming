#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_SIZE 30

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void printArray(int v[MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("%d ", v[i]);
        printf("\n");
    }
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int ranks;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    int part = 10;
    if (rank == 0) {
        int A[MATRIX_SIZE][MATRIX_SIZE];
        int vector[MATRIX_SIZE];
        int result[MATRIX_SIZE];
        for (int i = 0; i < MATRIX_SIZE; i++) {
            vector[i] = i;
            for (int j = 0; j < MATRIX_SIZE; j++) {
                A[i][j] = i + j;
            }
        }
        printMatrix(A);
        printArray(vector);
        int n = 1;
        for (int i = 0; i < MATRIX_SIZE; i += part) {
            MPI_Send(&vector, MATRIX_SIZE, MPI_INT, n, n, MPI_COMM_WORLD);
            for (int j = 0; j < part; j++) {
                MPI_Send(&A[i + j][0], MATRIX_SIZE, MPI_INT, n, n*j+n, MPI_COMM_WORLD);
            }
            n++;
        }

        n = 1;
        for (int i = 0; i < MATRIX_SIZE; i += part) {
            for (int j = 0; j < part; j++) {
                MPI_Recv(&result[i + j], 1, MPI_INT, n, n*j + n, MPI_COMM_WORLD, &status);
            }
            n++;
        }
        printArray(result);

    } else if (rank <= MATRIX_SIZE / part) {
        int vectorA[MATRIX_SIZE];
        int vectorB[MATRIX_SIZE];
        MPI_Recv(&vectorA[0], MATRIX_SIZE, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
        for (int j = 0; j < part; j++) {
            int c = 0;
            MPI_Recv(&vectorB[0], MATRIX_SIZE, MPI_INT,
                     0, rank*j + rank, MPI_COMM_WORLD, &status);
            for (int i = 0; i < MATRIX_SIZE; i++) {
                c += vectorA[i] * vectorB[i];
            }
            MPI_Send(&c, 1, MPI_INT, 0, rank*j + rank, MPI_COMM_WORLD);
        }

    }

    MPI_Finalize();
    return 0;

}
// 2.2. Реализовать умножение матрицы на столбец с = Ab, ci=sum(aik*bk)