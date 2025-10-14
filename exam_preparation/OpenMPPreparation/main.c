#include <stdio.h>
#include <omp.h>
#include <math.h>

int main(void) {
    printf("Hello, World!\n");

    return 0;
}
#define N 8
#define FILTER_SIZE 3

// void convolve_serial(float input[N][N], float output[N][N], float filter[FILTER_SIZE][FILTER_SIZE]) {
// #pragma omp parallel
//     {
// #pragma omp for collapse schedule(static)
//         {
//             for (int i = 1; i < N-1; i++) {
//                 for (int j = 1; j < N-1; j++) {
//                     float sum = 0.0;
//                     for (int ki = 0; ki < FILTER_SIZE; ki++) {
//                         for (int kj = 0; kj < FILTER_SIZE; kj++) {
//                             sum += input[i+ki-1][j+kj-1] * filter[ki][kj];
//                         }
//                     }
//                     output[i][j] = sum;
//                 }
//             }
//         }
//     }
// }

#define N 8
#define FILTER_SIZE 3
void convolve_serial(float input[N][N], float output[N][N], float filter[FILTER_SIZE][FILTER_SIZE]) {
#pragma omp parallel
    {
#pragma omp for collapse(2) schedule(static) ordered
        {
            for (int i = 1; i < N-1; i++) {
                for (int j = 1; j < N-1; j++) {
                    float sum = 0.0;
                    for (int ki = 0; ki < FILTER_SIZE; ki++) {
                        for (int kj = 0; kj < FILTER_SIZE; kj++) {
                            sum += input[i+ki-1][j+kj-1] * filter[ki][kj];
                        }
                    }
                    output[i][j] = sum;
#pragma omp ordered
                    {
                        printf("output:[%d][%d] = %d",i,j,output[i][j]);
                    }
                }
            }
        }
    }
}