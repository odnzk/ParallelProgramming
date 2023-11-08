#include <iostream>
#include <mpi.h>

#define VECTOR_SIZE 100

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double vectorA[VECTOR_SIZE];
    double vectorB[VECTOR_SIZE];
    if (rank == 0) {
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vectorA[i] = i + 1;
            vectorB[i] = i + 2;
        }
    }

    double localVectorA[VECTOR_SIZE / size];
    double localVectorB[VECTOR_SIZE / size];
    MPI_Scatter(vectorA, VECTOR_SIZE / size, MPI_DOUBLE, localVectorA, VECTOR_SIZE / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(vectorB, VECTOR_SIZE / size, MPI_DOUBLE, localVectorB, VECTOR_SIZE / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double localSum = 0.0;
    for (int i = 0; i < VECTOR_SIZE / size; i++) {
        localSum += localVectorA[i] * localVectorB[i];
    }

    double globalSum = 0.0;
    MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Scalar Product: " << globalSum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
// В следующих заданиях матрицы и векторы вещественных чисел задаются на нулевом процессе.
//Размер матриц не менее 100 x100 (можно взять больше кратным количеству процессов), размер
//векторов не менее 100 элементов. 1 задание = 2 балла
// 3.3.Написать программу умножения матрицы на вектор. Распределить матрицу равномерно вдоль
//строк, вектор передать всем процессам. После выполнения операции умножения на каждом из
//процессов необходимо выполнить сбор полученных локальных частей результирующего вектора на
//один процесс (с номером 0). Использовать функцию MPI_Bcast для рассылки вектора для
//распределения элементов матрицы использовать функцию MPI_Scatter, для сбора локальных частей
//результирующего вектора в глобальный вектор использовать функцию MPI_Gather.