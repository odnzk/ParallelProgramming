#include <iostream>
#include <cmath>
#include <omp.h>

int main() {
    int a[30] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};
    int size = 30;
    int base = 2;
    int decimal = 0;

    #pragma omp parallel for reduction(+: decimal)
    for (int i = 0; i < size; i++) {
        int power = size - i - 1;
        decimal += a[i] * pow(base, power);
    }
    printf("Result = %d", decimal);

    return 0;
}
