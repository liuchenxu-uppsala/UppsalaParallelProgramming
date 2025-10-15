//
// Created by Chenx on 2025/10/15.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
#define ROWS 1000
#define COLS 1000
double arr[ROWS][COLS];
int main() {
#pragma omp parallel
    {
#pragma omp for schedule(static) collapse(2)
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < ROWS; j++) {
                arr[i][j] = sin(i) * cos(j);
            }
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            printf("arr[%d][%d]:%lf\n",i,j,arr[i][j]);
        }
    }
    return 0;
}