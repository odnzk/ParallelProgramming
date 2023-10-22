#include <stdio.h>
#include <mpi.h>
#include <cstdlib>
#include <ctime>

int main(int argc, char **argv)
{
    int rank;
    int threads;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &threads);

    if (rank == 1) {
        const int LENGTH = 10;
        int a[LENGTH];
        srand(static_cast<unsigned int>(time(nullptr)));

        for (int i = 0; i < LENGTH; i++) {
            a[i] = rand() % 100;
        }

        printf("Array: ");
        for (int i = 0; i < LENGTH; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");

        for (int i = 0; i < threads; i++) {
            if (i != 1) {
                int send_count = LENGTH;
                MPI_Send(&send_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(a, send_count, MPI_INT, i, 1, MPI_COMM_WORLD);
            }
        }

    } else {
        int recv_count;
        MPI_Status status;
        MPI_Recv(&recv_count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        int* recv_data = new int[recv_count];
        MPI_Recv(recv_data, recv_count, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);

        printf("Received array: ");
        for (int i = 0; i < recv_count; i++) {
            printf("%d ", recv_data[i]);
        }
        printf("\n");

        delete[] recv_data;
    }

    MPI_Finalize();
}
// 1.4.Используя блокирующую операцию передачи сообщений (MPI_Send и MPI_Recv) выполнить
//пересылку данных одномерного массива, из процесса с номером 1 на остальные процессы группы.
//На процессах получателях до выставления функции приема (MPI_Recv) определить количество
//принятых данных (MPI_Probe). Выделить память под размер приемного буфера, после чего вызвать
//функцию MPI_Recv. Полученное сообщение выдать на экран.