#include <iostream>
#include <mpi.h>
#include <random>

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int VECTOR_SIZE = 100;
    constexpr int a = 3;
    constexpr int b = 2;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100);

    int vector_x[VECTOR_SIZE];
    int vector_y[VECTOR_SIZE];
    int vector_z[VECTOR_SIZE];

    if (rank == 0) {
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector_x[i] = dist(gen);
            vector_y[i] = dist(gen);
        }

        for (int i = 1; i < size; i++) {
            MPI_Send(vector_x, VECTOR_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(vector_y, VECTOR_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(vector_x, VECTOR_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(vector_y, VECTOR_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector_z[i] = a * vector_x[i] + b * vector_y[i];
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Recv(&vector_z[i * VECTOR_SIZE / size], VECTOR_SIZE / size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printf("Result: ");
        for (int i = 0; i < VECTOR_SIZE; i++) {
            printf("%d ", vector_z[i]);
        }
        printf("\n");
    }
    else {
        MPI_Send(vector_z, VECTOR_SIZE / size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
// 2.1. Написать программу, вычисляющую элементы вектора по формуле z = ax + by.
// Векторы x=(x1, x2...x100) и y=(y1,y2..y100)
//и задаются на нулевом процессе и равными блоками пересылаются остальным процессам, a,b -заданные числа.
