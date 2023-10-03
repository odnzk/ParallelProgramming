#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>

int main() {
    int beginning, end;
    std::vector<int> primeNumbers;
    printf("Beginning: ");
    std::cin >> beginning;
    printf("End: ");
    std::cin >> end;
    bool isPrimeNumber = true;
    int numberSqr = 0;

    #pragma omp parallel for private(isPrimeNumber, numberSqr)
    for (int i = beginning; i <= end; i++) {
        isPrimeNumber = true;
        numberSqr = sqrt(i);
        for (int j = 2; j <= numberSqr; j++) {
            if (i % j == 0) {
                isPrimeNumber = false;
                break;
            }
        }
        if (isPrimeNumber && i != 1) {
            #pragma omp critical
            primeNumbers.insert(primeNumbers.end(), i);
        }
    }
    for (int i = 0; i < primeNumbers.size(); i++) {
        printf("%d ", primeNumbers[i]);
    }
}
