#include <iostream>
#include <mpi.h>
#include <random>

int main(int argc, char **argv) {

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int const N = 120;
    int const part = 30;

    int x[N];
    int x_part[part];
    int y[N];
    int y_part[part];

    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            x[i] = i;
            y[i] = i * 3;
        }
    }

    MPI_Scatter(&x[0], part, MPI_INT, &x_part[0], part, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&y[0], part, MPI_INT, &y_part[0], part, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("x : ");
        for (int i = 0; i < N; ++i) {
            printf("%d ", x[i]);
        }
        printf("\n\n");

        printf("y : ");
        for (int i = 0; i < N; ++i) {
            printf("%d ", y[i]);
        }
        printf("\n\n");
    }

    int sum = 0;
    for (int i = 0; i < part; ++i) {
        sum += x_part[i] * y_part[i];
    }

    int result;
    MPI_Reduce(&sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result = %d\n\n", result);
    }

    MPI_Finalize();

    return 0;
}
// В следующих заданиях матрицы и векторы вещественных чисел задаются на нулевом процессе.
//Размер матриц не менее 100 x100 (можно взять больше кратным количеству процессов), размер
//векторов не менее 100 элементов. 1 задание = 2 балла
// 3.2.
//Написать программу вычисления скалярного произведения двух векторов . Для
//распределения элементов векторов по процессам использовать функцию MPI_Scatter. Для
//получения итоговой суммы на нулевом процессе использовать функцию MPI_Reduce с операцией
//MPI_Sum.
