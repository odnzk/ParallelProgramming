#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 100;
    const int part = 20;

    int b[N];
    int a[N][N];
    int a_part[part][N];

    srand(time(nullptr));

    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            b[i] = i;
            for (int j = 0; j < N; ++j) {
                a[i][j] = rand() % 5 + 1;
            }
        }
        printf("A: \n");
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }
        printf("\n\n");

        printf("b : ");
        for (int i = 0; i < N; ++i) {
            printf("%d ", b[i]);
        }
        printf("\n\n");
    }

    MPI_Bcast(b, N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(a, part * N, MPI_INT, a_part, part * N, MPI_INT, 0, MPI_COMM_WORLD);

    int result[part];

    for (int i = 0; i < part; i++) {
        result[i] = 0;
        for (int j = 0; j < N; j++) {
            result[i] += a_part[i][j] * b[j];
        }
    }

    int resAll[N];

    MPI_Gather(result, part, MPI_INT, resAll, part, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Res: ");
        for (int i = 0; i < N; ++i) {
            printf("%d ", resAll[i]);
        }
        printf("\n");
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