//
// Created by Chenx on 2025/10/15.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
#define N 100
int arr[N];
long shared_result= 0;
int main() {
    for (int index = 0; index < N; index++) {
        arr[index] = index;
    }
    #pragma omp parallel num_threads(16)
    {
    #pragma omp for reduction(+:shared_result) schedule(static)
        for (int index = 0; index < N; index++){
            shared_result += arr[index] * arr[index];
        }
    }
    printf("shared_result:%ld\n",shared_result);
}