#include <iostream>
#include <cmath>
#include <mpi.h>

#define MATRIX_SIZE 100

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double matrix[MATRIX_SIZE][MATRIX_SIZE];
    if (rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                matrix[i][j] = i + j + 1;
            }
        }
    }

    double localMatrix[MATRIX_SIZE / size][MATRIX_SIZE];
    MPI_Scatter(matrix, (MATRIX_SIZE / size) * MATRIX_SIZE, MPI_DOUBLE, localMatrix, (MATRIX_SIZE / size) * MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double localNorms[MATRIX_SIZE / size];
    for (int i = 0; i < MATRIX_SIZE / size; i++) {
        localNorms[i] = 0.0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            localNorms[i] += std::abs(localMatrix[i][j]);
        }
    }

    double maxLocalNorm = 0.0;
    MPI_Reduce(localNorms, &maxLocalNorm, MATRIX_SIZE / size, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    double globalMaxNorm = 0.0;
    MPI_Reduce(&maxLocalNorm, &globalMaxNorm, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Maximum Norm: " << globalMaxNorm << std::endl;
    }

    MPI_Finalize();
    return 0;
}

