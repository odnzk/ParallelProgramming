#include <iostream>
#include <omp.h>

int main() {
    int num_threads1 = 3;
    int num_threads2 = 1;

    #pragma omp parallel if(num_threads1 > 1) num_threads(num_threads1)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        std::cout << "Space 1 - thread: " << thread_id << ", thread count: " << total_threads << std::endl;
    }

    #pragma omp parallel if(num_threads2 > 1) num_threads(num_threads2)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        std::cout << "Space 2 - thread: " << thread_id << ", thread count: " << total_threads << std::endl;
    }

    return 0;
}
