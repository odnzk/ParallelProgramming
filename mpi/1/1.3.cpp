#include <iostream>
#include <mpi.h>
#include <random>

int main(int argc, char** argv)
{
    int rank;
    int const SIZE = 10;
    int const msgtag = 99;
    int a[SIZE];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 50);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < SIZE; ++i) {
            a[i] = dist(gen);
        }
        MPI_Send(a, SIZE, MPI_INT, 1, msgtag, MPI_COMM_WORLD);
    }
    if (rank == 1) {
        MPI_Recv(a, SIZE, MPI_INT, 0, msgtag, MPI_COMM_WORLD, &status);
        for (int i = 0; i < SIZE; i++) {
            printf("a[i]: %d ", a[i]);
        }
    }

    MPI_Finalize();
}
// 1.3.Написать программу, запустить ее на 2-х процессах.
// На нулевом процессе задать массив a из 10
//элементов, значения сгенерировать случайным образом.
// Переслать этот массив целиком первому
//процессу с помощью функции MPI_Send.
// Распечатать на первом процессе принятый массив.
