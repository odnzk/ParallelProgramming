#include <iostream>
#include <omp.h>

int main() {
    int a[10] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    int b[10] = {10, 17, 6, 22, 9, 14, 3, 8, 12, 19};
    int min_a = a[0];
    int max_b = b[0];

    #pragma omp parallel num_threads(2)
    {
        int thread_id = omp_get_thread_num();

        if (thread_id == 0) {
            for (int i = 1; i < 10; i++) {
                if (a[i] < min_a) {
                    min_a = a[i];
                }
            }
        } else if (thread_id == 1) {
            for (int i = 1; i < 10; i++) {
                if (b[i] > max_b) {
                    max_b = b[i];
                }
            }
        }
    }

    std::cout << "min from a: " << min_a << std::endl;
    std::cout << "max from b: " << max_b << std::endl;

    return 0;
}
