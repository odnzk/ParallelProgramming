#include "mpi.h"
#include <iostream>

int main(int argc, char** argv)
{
    int nPr, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &nPr);
    printf("Hello world!\n");
    MPI_Finalize();
}
// 1.1.Написать MPI программу, печатающую "Hello, World!". Запустить программу на 4 процессах.