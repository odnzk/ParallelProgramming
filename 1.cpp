#include <iostream>
#include <omp.h>

int main() {
    #pragma omp parallel num_threads(8)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        std::cout << "Thread: " << thread_id << ", Thread count: " << total_threads << ", HelloWorld" << std::endl;
    }

    return 0;
}
