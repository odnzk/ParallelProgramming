#include <iostream>

int main() {
    int number = 0;

    #pragma omp parallel for reduction(+:number)
    for (int i = 0; i < 210; i++) {
        number += 2*i + 1;
    }

    printf("Number = %d", number);
}
