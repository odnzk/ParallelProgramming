#include <iostream>
#include <random>
#include <omp.h>

int main() {
    const int size = 30;
    int a[size];

    // init array with random numbers
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        a[i] = dis(gen);
    }

    // calc number of elements that are multiples of 9
    int count = 0;
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        #pragma omp atomic
        if (a[i] % 9 == 0) {
            count++;
        }
    }
    std::cout << "Count: " << count << std::endl;

    return 0;
}
