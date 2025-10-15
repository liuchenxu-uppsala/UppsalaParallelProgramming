//
// Created by Chenx on 2025/10/15.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
#define SIZE 1024
double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
int main() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = i + j + 2;
            B[i][j] = i + j + 5;
            C[i][j] = 0;
        }
    }
    #pragma omp parallel num_threads(16)
        {
    #pragma omp for schedule(static) collapse(2)
            for (int ci = 0; ci < SIZE; ci++) {
                for (int cj = 0; cj < SIZE; cj++) {
                    double local_sum = 0;
                    for (int index = 0; index < SIZE; index++) {
                        local_sum = local_sum + A[ci][index] * B[index][cj];
                    }
                    C[ci][cj] = local_sum;
                }
            }
        }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("C[%d][%d]:%lf\n",i,j,C[i][j]);
        }
    }
    return 0;
}