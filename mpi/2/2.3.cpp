#include <stdio.h>
#include <mpi.h>

#define MATRIX_SIZE 10

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int ranks;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int matrixA[MATRIX_SIZE][MATRIX_SIZE];
    int matrixB[MATRIX_SIZE][MATRIX_SIZE];
    int result[MATRIX_SIZE][MATRIX_SIZE];
    MPI_Status status;
    int part = 5;
    if (rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrixA[i][j] = i + j;
                matrixB[i][j] = j - i;
            }
        }
        printMatrix(matrixA);
        printMatrix(matrixB);
        int n = 1;
        for (int i = 0; i < MATRIX_SIZE; i += part) {
            for (int j = 0; j < part; j++) {
                MPI_Send(&matrixA[i+j][0], MATRIX_SIZE, MPI_INT, n, n*j+n, MPI_COMM_WORLD);
                MPI_Send(&matrixB[i+j][0], MATRIX_SIZE, MPI_INT, n, n*j+n, MPI_COMM_WORLD);
            }
            n++;
        }
        n = 1;
        for (int i = 0; i < MATRIX_SIZE; i += part) {
            for (int j = 0; j < part; j++) {
                MPI_Recv(&result[i+j][0], MATRIX_SIZE, MPI_INT, n, n*j + n, MPI_COMM_WORLD, &status);
            }
            n++;
        }

    } else if (rank <= MATRIX_SIZE / part)  {
        int a[MATRIX_SIZE];
        int b[MATRIX_SIZE];
        for (int j = 0; j < part; j++) {
            MPI_Recv(&a[0], MATRIX_SIZE, MPI_INT, 0, rank*j + rank, MPI_COMM_WORLD, &status);
            MPI_Recv(&b[0], MATRIX_SIZE, MPI_INT, 0, rank*j + rank, MPI_COMM_WORLD, &status);
            int res[MATRIX_SIZE];
            for (int k = 0; k < MATRIX_SIZE; k++) {
                res[k] = a[k] * b[k];
            }
            MPI_Send(&res, MATRIX_SIZE, MPI_INT, 0, rank*j + rank, MPI_COMM_WORLD);
        }

    }

    if (rank == 0) {
        printf("Result matrix:\n");
        printMatrix(result);
    }

    MPI_Finalize();
    return 0;
}
// 2.3. Реализовать поэлементное умножение матриц