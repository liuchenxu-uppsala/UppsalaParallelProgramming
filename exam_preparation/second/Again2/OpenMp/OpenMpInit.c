//
// Created by Chenx on 2025/10/22.
//
#include <stdio.h>
#include<omp.h>
#define ARR_LEN 10
int a[ARR_LEN][ARR_LEN];
int main() {
#pragma omp parallel
    {
#pragma omp for schedule(dynamic,2) collapse(2) nowait
        for (int i = 0; i < ARR_LEN; i++) {
            for (int j = 0; j < ARR_LEN; j++) {
                a[i][j] = omp_get_thread_num();
            }
        }
#pragma omp barrier
#pragma omp single
        printf("=== all threads reached barrier ===\n");
        printf("Thread:%d barrier\n",omp_get_thread_num());
#pragma omp for schedule(static) ordered
        for (int in = 0; in < 100; in++) {
#pragma omp ordered
            printf("Thread:%d,in:%d\n",omp_get_thread_num(),in);
        }
    }
}