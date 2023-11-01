#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define MATRIX_SIZE 100

using namespace std;

vector<vector<int>> generateRandomMatrix(int size) {
    vector<vector<int>> matrix(size, vector<int>(size, 0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
    return matrix;
}

int main(int argc, char** argv) {
    int rank, numProcesses;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    vector<vector<int>> A(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    vector<vector<int>> B(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    vector<int> C(MATRIX_SIZE, 0);

    if (rank == 0) {
        A = generateRandomMatrix(MATRIX_SIZE);
        B = generateRandomMatrix(MATRIX_SIZE);
    }

    MPI_Bcast(&B[0][0], MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    int rowsPerProcess = MATRIX_SIZE / numProcesses;
    int startRow = rank * rowsPerProcess;
    int endRow = startRow + rowsPerProcess;

    vector<int> localA(rowsPerProcess * MATRIX_SIZE, 0);
    MPI_Scatter(&A[0][0], rowsPerProcess * MATRIX_SIZE, MPI_INT, &localA[0], rowsPerProcess * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            for (int k = 0; k < MATRIX_SIZE; k++) {
                C[i] += localA[(i - startRow) * MATRIX_SIZE + k] * B[k][j];
            }
        }
    }

    vector<int> globalC(MATRIX_SIZE, 0);
    MPI_Gather(&C[startRow], rowsPerProcess, MPI_INT, &globalC[0], rowsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Matrix C:\n");
        for (int i = 0; i < MATRIX_SIZE; i++) {
            printf("%d ", globalC[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
// 2.2. Реализовать умножение матрицы на столбец с = Ab, ci=sum(aik*bk)