#include <iostream>
#include <random>
#include <omp.h>

int main() {
    int length = 100;
    int num_threads = 3;
    int* array = new int[length];

    // init random array
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < length; i++) {
        array[i] = dis(gen);
    }

    // calc max value of elements that are multiples of 7
    int max_val = 0;
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < length; i++) {
        if (array[i] % 7 == 0) {
            #pragma omp critical
            {
                if (array[i] > max_val) {
                    max_val = array[i];
                }
            }
        }
    }
    std::cout << "Max: " << max_val << std::endl;
  
    return 0;
}
