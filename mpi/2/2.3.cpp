#include <mpi.h>
#include <iostream>
#include <vector>

#define MATRIX_SIZE 100
#define BLOCK_SIZE 10

// 2.3. Реализовать поэлементное умножение матриц
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

    if (rank == 0) {
        A = generateRandomMatrix(MATRIX_SIZE);
        B = generateRandomMatrix(MATRIX_SIZE);
    }

    MPI_Bcast(&B[0][0], MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    int numBlocks = MATRIX_SIZE / BLOCK_SIZE;

    int blocksPerProcess = numBlocks / numProcesses;
    int startBlock = rank * blocksPerProcess;
    int endBlock = startBlock + blocksPerProcess;

    vector<vector<int>> localC(BLOCK_SIZE, vector<int>(BLOCK_SIZE, 0));
    for (int i = startBlock; i < endBlock; i++) {
        for (int j = 0; j < numBlocks; j++) {
            for (int k = 0; k < numBlocks; k++) {
                for (int ii = 0; ii < BLOCK_SIZE; ii++) {
                    for (int jj = 0; jj < BLOCK_SIZE; jj++) {
                        for (int kk = 0; kk < BLOCK_SIZE; kk++) {
                            localC[ii][jj] += A[i * BLOCK_SIZE + ii][k * BLOCK_SIZE + kk] * B[k * BLOCK_SIZE + kk][j * BLOCK_SIZE + jj];
                        }
                    }
                }
            }
        }
    }

    vector<vector<int>> globalC(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0));
    MPI_Allgather(&localC[0][0], BLOCK_SIZE * BLOCK_SIZE, MPI_INT, &globalC[0][0], BLOCK_SIZE * BLOCK_SIZE, MPI_INT, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Matrix C: " << endl;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                cout << globalC[i][j] << " ";
            }
            cout << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
