//
// Created by Chenx on 2025/10/15.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>
#define N 1024
int* input;
int* output;
int main() {
    input = (int*)malloc(sizeof(int) * N);
    output = (int*)malloc(sizeof(int) * N);
    for (int i = 0; i < N; i++) {
        input[i] = i;
    }
#pragma omp parallel
    {
#pragma omp for schedule(dynamic)
        for (int i = 0; i < N; i++) {
            int local_sum = 0;
            for (int index = 0; index<=i;index++) {
                local_sum+=input[index];
            }
            output[i] = local_sum;
        }
    }
    for (int i = 0; i < N; i++) {
        printf("output[%d]:%d\n",i,output[i]);
    }
    return 0;
}