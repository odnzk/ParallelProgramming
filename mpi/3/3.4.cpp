#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int result;
    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 120;
    const int part = 20;

    int A[N][N];
    int a[part][N];

    srand(time(nullptr));

    if (rank == 0) {
        printf("A:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = i + 1;
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    MPI_Scatter(&A[rank * part][0], part * N, MPI_INT, &a[0][0], part * N, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    int max = 0;
    for (int i = 0; i < part; i++) {
        sum = 0;
        for (int j = 0; j < N; j++) {
            sum += abs(a[i][j]);
        }
        if (sum > max) {
            max = sum;
        }
    }

    MPI_Reduce(&max, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("||A||: %d\n", result);
    }

    MPI_Finalize();

    return 0;
}