#include <iostream>
#include <omp.h>

int main() {
    int a[12], b[12], c[12];
    omp_set_num_threads(3);
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < 12; i++) {
        a[i] = i + 1;
        b[i] = (i + 1) * 2;
        std::cout << "Thread: " << omp_get_thread_num() << " initialized a[" << i << "] = " << a[i] << " and b[" << i << "] = " << b[i] << std::endl;
    }
  
    omp_set_num_threads(4); 
    #pragma omp parallel for schedule(dynamic, 3)
    for (int i = 0; i < 12; i++) {
        c[i] = a[i] + b[i];
        std::cout << "Thread: " << omp_get_thread_num() << " calculated c[" << i << "] = " << c[i] << std::endl;
    }

    return 0;
}
