//
// Created by Chenx on 2025/10/27.
//
#include <stdio.h>
#include<omp.h>
#define ROW 10
#define COLUMN 10
int A[ROW][COLUMN];

int main() {
#pragma omp parallel
    {
#pragma omp for collapse(2) schedule(dynamic,5) ordered
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COLUMN; j++) {
                A[i][j] = omp_get_thread_num();
                #pragma omp ordered
                {
                    printf("A[%d][%d]=%d\n",i,j,A[i][j]);
                }
            }
        }
    }
}