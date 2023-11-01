#include <iostream>
#include <string>
#include "mpi.h"
#include <algorithm>

#include <mpi.h>
#include <iostream>
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

    if (rank == 0) {
        A = generateRandomMatrix(MATRIX_SIZE);
    }

    int rowsPerProcess = MATRIX_SIZE / numProcesses;
    int startRow = rank * rowsPerProcess;
    int endRow = startRow + rowsPerProcess;

    vector<vector<int>> localA(rowsPerProcess, vector<int>(MATRIX_SIZE, 0));
    MPI_Scatter(&A[0][0], rowsPerProcess * MATRIX_SIZE, MPI_INT, &localA[0][0], rowsPerProcess * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    vector<vector<int>> localAT(MATRIX_SIZE, vector<int>(rowsPerProcess, 0));
    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            localAT[j][i - startRow] = localA[i - startRow][j];
        }
    }

    vector<vector<int>> globalAT(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    MPI_Allgather(&localAT[0][0], rowsPerProcess * MATRIX_SIZE, MPI_INT, &globalAT[0][0], rowsPerProcess * MATRIX_SIZE, MPI_INT, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Matrix A Transpose: " << endl;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                cout << globalAT[i][j] << " ";
            }
            cout << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
// Реализовать транспонирование матрицы A.


