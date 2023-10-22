#include <iostream>
#include <vector>
#include <omp.h>

// function to perform matrix-vector multiplication
std::vector<int> matrixVectorMultiplication(const std::vector<std::vector<int>>& matrix, const std::vector<int>& vector) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    std::vector<int> result(rows, 0);

    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    return result;
}

int main() {
    // matrix
    std::vector<std::vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    // vector
    std::vector<int> vector = {1, 2, 3};

    //  multiplication with serial implementation
    std::vector<int> serialResult = matrixVectorMultiplication(matrix, vector);
  
    // multiplication with parallel implementation
    std::vector<int> parallelResult;
    double startTime = omp_get_wtime();
    parallelResult = matrixVectorMultiplication(matrix, vector);
    double endTime = omp_get_wtime();

  
    std::cout << "Serial result: ";
    for (int i = 0; i < serialResult.size(); i++) {
        std::cout << " " << serialResult[i];
    }
    std::cout << std::endl;

    std::cout << "Parallel result: ";
    for (int i = 0; i < parallelResult.size(); i++) {
        std::cout << " " << parallelResult[i];
    }
    std::cout << std::endl;

    // Calc execution time of parallel implementation
    double executionTime = endTime - startTime;
    std::cout << "Execution time of the parallel implementation: " << executionTime << " seconds" << std::endl;

    return 0;
}

