//
// Created by Chenx on 2025/10/27.
//
#include "omp.h"
#include <stdio.h>
#define NUM 1000000
long A[NUM];
long B[NUM];

int main() {
#pragma omp parallel
    {
#pragma omp for nowait
        for (long i = 0; i < NUM; i++) {
            A[i] = i;
        }
#pragma omp barrier
#pragma omp for
        for (long i = 0; i < NUM; i++) {
            B[i] = A[i] + 1;
        }
    }
}