#include <iostream>
#include <mpi.h>
#include <random>
#include <math.h>

int main(int argc, char** argv)
{
    int rank;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 50);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int count;
    int msgtag = 9

    const int SIZE = 100;
    const int BLOCK = 5;

    if (rank == 0) {
        int x[SIZE];
        int y[SIZE];
        int z[SIZE];

        for (int i = 0; i < SIZE; ++i) {
            x[i] = dist(gen);
            y[i] = dist(gen);
        }

        printf("x: ");
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", x[i]);
        }
        printf("\ny: ");
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", y[i]);
        }
        printf("\n");

        int n = 1;
        for (int i = 0; i < SIZE; i += BLOCK, n++) {
            MPI_Send(&x[i], BLOCK, MPI_INT, n, msgtag, MPI_COMM_WORLD);
            MPI_Send(&y[i], BLOCK, MPI_INT, n, msgtag, MPI_COMM_WORLD);
        }

        n = 1;
        for (int i = 0; i < SIZE; i += BLOCK, n++) {
            MPI_Recv(&z[i], BLOCK, MPI_INT, n, msgtag, MPI_COMM_WORLD, &status);
        }

        printf("z: ");
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", z[i]);
        }
    } else {
        MPI_Probe(0, msgtag, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        
        int x[BLOCK];
        int y[BLOCK];
        int z[BLOCK];
        MPI_Recv(&x[0], count, MPI_INT, 0, msgtag, MPI_COMM_WORLD, &status);
        MPI_Recv(&y[0], count, MPI_INT, 0, msgtag, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; ++i) {
            z[i] = x[i] * y[i];
        }
        MPI_Send(&z, count, MPI_INT, 0, msgtag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
// 2.1. Написать программу, вычисляющую элементы вектора по формуле z = ax + by.
// Векторы x=(x1, x2...x100) и y=(y1,y2..y100)
//и задаются на нулевом процессе и равными блоками пересылаются остальным процессам, a,b -заданные числа.