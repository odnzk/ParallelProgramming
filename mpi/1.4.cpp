#include <iostream>
#include <mpi.h>
#include <random>

int main(int argc, char** argv)
{
    int rank, count;
    const int LENGTH = 10;
    const int msgtag = 99;
    int a[LENGTH];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 50);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1) {
        for (int i = 0; i < 10; ++i) {
            a[i] = dist(gen);
        }
        MPI_Send(&a, 10, MPI_INT, 0, msgtag, MPI_COMM_WORLD);
        MPI_Send(&a, 10, MPI_INT, 2, msgtag, MPI_COMM_WORLD);
        MPI_Send(&a, 10, MPI_INT, 3, msgtag, MPI_COMM_WORLD);
    } else {
        MPI_Probe(1, msgtag, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        MPI_Recv(&a, count, MPI_INT, 1, msgtag, MPI_COMM_WORLD, &status);

        for (int i = 0; i < 10; i++) {
            printf("a[i]: %d \n", a[i]);
        }
    }

    MPI_Finalize();
}
// 1.4.Используя блокирующую операцию передачи сообщений (MPI_Send и MPI_Recv) выполнить
//пересылку данных одномерного массива, из процесса с номером 1 на остальные процессы группы.
//На процессах получателях до выставления функции приема (MPI_Recv) определить количество
//принятых данных (MPI_Probe). Выделить память под размер приемного буфера, после чего вызвать
//функцию MPI_Recv. Полученное сообщение выдать на экран.