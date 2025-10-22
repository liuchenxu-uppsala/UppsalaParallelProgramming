//
// Created by Chenx on 2025/10/22.
//
#include <stdio.h>
#include<omp.h>
#define N 4
#define M 5
int a[N][M] = {
    {1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10},
    {11, 12, 13, 14, 15},
    {16, 17, 18, 19, 20}
};

int global_sum = 0;

int main() {
#pragma omp parallel
    {
#pragma omp for collapse(2) schedule(dynamic,4) reduction(+:global_sum)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                printf("Thread %d handles a[%d][%d] = %d\n",omp_get_thread_num(),i,j,a[i][j]);
                global_sum += a[i][j] * a[i][j];
            }
        }
    }
    printf("global_sum:%d\n",global_sum);
}