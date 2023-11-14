#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank,ranks;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int const n = 120;
    int const part = 30;
    int x[n];
    int y[part];

    if (rank == 0) {
        printf("x: ");
        for (int i = 0; i < n; i++) {
            x[i] = i * 3;
            printf("%d ", x[i]);
        }
        printf("\n\n");
    }

    MPI_Scatter(&x[0], part, MPI_INT, &y[0], part, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    for (int i = 0; i < part; i++) {
        sum += abs(y[i]);
    }

    int result;
    MPI_Reduce(&sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("||x||: %d\n\n", result);
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