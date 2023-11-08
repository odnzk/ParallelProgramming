#include <iostream>
#include <cmath>
#include <mpi.h>

#define VECTOR_SIZE 100

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double vector[VECTOR_SIZE];
    if (rank == 0) {
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector[i] = i + 1;
        }
    }

    double localVector[VECTOR_SIZE / size];
    MPI_Scatter(vector, VECTOR_SIZE / size, MPI_DOUBLE, localVector, VECTOR_SIZE / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double localNorm = 0.0;
    for (int i = 0; i < VECTOR_SIZE / size; i++) {
        localNorm += localVector[i] * localVector[i];
    }

    double globalNorm = 0.0;
    MPI_Reduce(&localNorm, &globalNorm, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Norm: " << std::sqrt(globalNorm) << std::endl;
    }

    MPI_Finalize();
    return 0;
}
// В следующих заданиях матрицы и векторы вещественных чисел задаются на нулевом процессе.
//Размер матриц не менее 100 x100 (можно взять больше кратным количеству процессов), размер
//векторов не менее 100 элементов. 1 задание = 2 балла
// 3.1. Написать программу вычисления нормы вектора Для
//распределения элементов вектора по процессам использовать функцию MPI_Scatter. Для
//получения итоговой суммы на нулевом процессе использовать функцию MPI_Reduce с операцией
//MPI_Sum.