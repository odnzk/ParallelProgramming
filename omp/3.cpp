#include <iostream>
#include <omp.h>

int main() {
    int a = 5;
    int b = 10;

    std::cout << "a = " << a << ", b = " << b << std::endl;

    #pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        int thread_id = omp_get_thread_num();
        a += thread_id;
        b += thread_id;
        std::cout << "space 1 - thread: " << thread_id << ", a = " << a << ", b = " << b << std::endl;
    }

    std::cout << "space 1: a = " << a << ", b = " << b << std::endl;

    #pragma omp parallel num_threads(4) shared(a) private(b)
    {
        int thread_id = omp_get_thread_num();
        a -= thread_id;
        b -= thread_id;
        std::cout << "space 2 - thread: " << thread_id << ", a = " << a << ", b = " << b << std::endl;
    }

    std::cout << "space 2: a = " << a << ", b = " << b << std::endl;

    return 0;
}
