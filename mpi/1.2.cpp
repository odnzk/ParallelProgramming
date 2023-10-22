#include <iostream>
#include "mpi.h"

int main(int argc, char** argv)
{
    int nPr, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &nPr);
    printf("Process number: %d, size: %d\n", nPr, size);
    MPI_Finalize();
}
// 1.2.Написать программу, печатающую номер процесса и общее количество процессов в коммуникаторе MPI_COMM_WORLD.
