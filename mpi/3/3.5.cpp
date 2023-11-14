#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int const n = 120;
    int const part = 40;
    int x[n];
    int y[part];

    if (rank == 0) {
        printf("x: ");
        for (int i = 0; i < n; i++) {
            x[i] = i + 1;
            printf("%d ", x[i]);
        }
        printf("\n\n");
    }

    MPI_Scatter(&x[0], part, MPI_INT, &y[0], part, MPI_INT, 0, MPI_COMM_WORLD);

    int locMax = 0;
    for (int i = 0; i < part; i++) {
        if (y[i] > locMax) {
            locMax = y[i];
        }
    }

    int locRes[2];
    int globalRes[2];
    locRes[0] = locMax;
    locRes[1] = rank;
    int allRes[size][2];

    MPI_Gather(locRes, 2, MPI_INT, allRes, 2, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Reduce(&locRes, &globalRes, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            printf("Local max %d: %d\n", allRes[i][1], allRes[i][0]);
        }
        printf("Global max: %d, rank %d\n\n", globalRes[0], globalRes[1]);
    }

    MPI_Finalize();

    return 0;
}
// Дан вектор x. Для распределения элементов вектора по процессам использовать функцию
//MPI_Scatter. Используя функцию MPI_Reduce для операции MPI_MAXLOC, найти максимальное
//значение среди элементов x. Вывести в главном процессе вначале все локальные максимумы (для
//каждого процесса свой максимум для части вектора x), а затем -- ранги содержащих их процессов