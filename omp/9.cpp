#include <iostream>
#include <random>
#include <omp.h>

int main() {
    const int rows = 6;
    const int cols = 8;
    int d[rows][cols];

    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    // init array with random numbers
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            d[i][j] = dis(gen);
        }
    }

    // min and max elements in array
    int min_val = d[0][0];
    int max_val = d[0][0];
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            #pragma omp critical
            {
                if (d[i][j] < min_val) {
                    min_val = d[i][j];
                }
                if (d[i][j] > max_val) {
                    max_val = d[i][j];
                }
            }
        }
    }
    std::cout << "Min: " << min_val << std::endl;
    std::cout << "Max: " << max_val << std::endl;

    return 0;
}
