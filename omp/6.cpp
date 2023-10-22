#include <iostream>
#include <omp.h>

int main() {
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int b[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    int sum_a = 0;
    int sum_b = 0;
  
    #pragma omp parallel for reduction(+:sum_a, sum_b)
    for (int i = 0; i < 10; i++) {
        sum_a += a[i];
        sum_b += b[i];
    }

    float mean_a = static_cast<float>(sum_a) / 10;
    float mean_b = static_cast<float>(sum_b) / 10;

    if (mean_a > mean_b) {
        std::cout << "Mean value of array 'a' is greater than mean value of array 'b'" << std::endl;
    } else if (mean_a < mean_b) {
        std::cout << "Mean value of array 'b' is greater than mean value of array 'a'" << std::endl;
    } else {
        std::cout << "Mean values of array 'a' and array 'b' are equal" << std::endl;
    }

    return 0;
}
