#include <iostream>
#include <string>
#include "mpi.h"
#include <algorithm>

// 2.1. Написать программу, вычисляющую элементы вектора по формуле z = ax + by.
// Векторы x=(x1, x2...x100) и y=(y1,y2..y100)
//и задаются на нулевом процессе и равными блоками пересылаются остальным процессам, a,b -заданные числа.
int main(int argc, char** argv)
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int senderRank = 0;
    const int msgtag = 0;
    const int a = 5;
    const int b = 7;
    const int n = 20;

    int part = n / (size - 1);
    int sizeForLast = part;
    if (n % (size - 1) > 0) {
        part++;
        sizeForLast = n % part;
    }

    if (rank == senderRank) {
        int* x = new int[n];
        int* y = new int[n];
        int* z = new int[n];
        for (int i = 0; i < n; ++i) {
            x[i] = rand(0, 10);
            y[i] = rand(0, 10);
        }

        printf("x: ");
        printArray(x, n);
        printf("y: ");
        printArray(y, n);

        int start = 0;
        for (int i = 0; i < size; i++) {
            if (i == senderRank) continue;
            int sizeToTransfer = part;
            if (i == size - 1) sizeToTransfer = sizeForLast;

            MPI_Send(&x[start], sizeToTransfer, MPI_INT, i, msgtag, MPI_COMM_WORLD);
            MPI_Send(&y[start], sizeToTransfer, MPI_INT, i, msgtag, MPI_COMM_WORLD);
            start += part;
        }
        start = 0;
        for (int i = 0; i < size; i++) {
            if (i == senderRank) continue;
            int sizeToTransfer = part;
            if (i == size - 1) sizeToTransfer = sizeForLast;

            MPI_Recv(&z[start], sizeToTransfer, MPI_INT, i, msgtag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            start += part;
        }

        printf("z: ");
        printArray(z, n);
    }
    else
    {
        int count;
        MPI_Status status;

        MPI_Probe(senderRank, msgtag, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);

        int* x = new int[count];
        int* y = new int[count];
        int* z = new int[count];

        MPI_Recv(&x[0], count, MPI_INT, senderRank, msgtag, MPI_COMM_WORLD, &status);
        MPI_Recv(&y[0], count, MPI_INT, senderRank, msgtag, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; i++) {
            z[i] = a * x[i] + b * y[i];
        }

        MPI_Send(&z[0], count, MPI_INT, senderRank, msgtag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}

void printArray(int* array, int count) {
    for (int i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
