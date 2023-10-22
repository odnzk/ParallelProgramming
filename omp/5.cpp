#include <iostream>
#include <omp.h>

int main() {
    int d[6][8];
    srand(time(NULL));

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            d[i][j] = rand() % 100;
        }
    }

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            int sum = 0;
            int count = 0;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    sum += d[i][j];
                    count++;
                }
            }

            double average = static_cast<double>(sum) / count;
            std::cout << "Thread " << omp_get_thread_num() << ": average: " << average << std::endl;
        }

        #pragma omp section
        {
            int min_value = d[0][0];
            int max_value = d[0][0];
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    if (d[i][j] < min_value) {
                        min_value = d[i][j];
                    }

                    if (d[i][j] > max_value) {
                        max_value = d[i][j];
                    }
                }
            }

            std::cout << "Thread " << omp_get_thread_num() << ": min: " << min_value << std::endl;
            std::cout << "Thread " << omp_get_thread_num() << ": max: " << max_value << std::endl;
        }

        #pragma omp section
        {
            int count_multiple_of_3 = 0;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    if (d[i][j] % 3 == 0) {
                        count_multiple_of_3++;
                    }
                }
            }

            std::cout << "Thread " << omp_get_thread_num() << ": count of the numbers are multiples of 3: " << count_multiple_of_3 << std::endl;
        }
    }

    return 0;
}
