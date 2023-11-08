#include <iostream>
#include <mpi.h>

#define VECTOR_SIZE 100

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double* vector = nullptr;
    double localMax = 0.0;
    int localMaxIndex = -1;

    if (rank == 0) {
        vector = new double[VECTOR_SIZE];
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector[i] = i + 1;
        }
    }

    double localVector[VECTOR_SIZE / size];
    MPI_Scatter(vector, VECTOR_SIZE / size, MPI_DOUBLE, localVector, VECTOR_SIZE / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < VECTOR_SIZE / size; i++) {
        if (localVector[i] > localMax) {
            localMax = localVector[i];
            localMaxIndex = i + rank * (VECTOR_SIZE / size);
        }
    }

    struct {
        double value;
        int index;
    } localMaxData, globalMaxData;

    localMaxData.value = localMax;
    localMaxData.index = localMaxIndex;

    MPI_Reduce(&localMaxData, &globalMaxData, 1, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

    std::cout << "Process " << rank << ": Local Max Value = " << localMaxData.value << ", Local Max Index = " << localMaxData.index << std::endl;

    if (rank == 0) {
        std::cout << "Processes containing the global maximum value: ";
        for (int i = 0; i < size; i++) {
            if (globalMaxData.index >= i * (VECTOR_SIZE / size) && globalMaxData.index < (i + 1) * (VECTOR_SIZE / size)) {
                std::cout << i << " ";
            }
        }
        std::cout << std::endl;
    }

    if (rank == 0) {
        delete[] vector;
    }

    MPI_Finalize();
    return 0;
}
// 3.5. Дан вектор Для распределения элементов вектора по процессам использовать функцию
//MPI_Scatter. Используя функцию MPI_Reduce для операции MPI_MAXLOC, найти максимальное
//значение среди элементов x. Вывести в главном процессе вначале все локальные максимумы (для
//каждого процесса свой максимум для части вектора x), а затем -- ранги содержащих их процессов