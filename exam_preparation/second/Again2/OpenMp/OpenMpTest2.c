//
// Created by Chenx on 2025/10/22.
//
#include <stdio.h>
#include<omp.h>
#define N 1000
#define M 1000
#define L 100
int a[N][M][L];

int main() {
    long shared_value1 = 0;
    long shared_value2 = 0;
    long shared_value3 = 0;
#pragma omp parallel
    {
#pragma omp for collapse(3) schedule(static)
        for (int n = 0; n < N; n++) {
            for (int m = 0; m < M; m++) {
                for (int l = 0; l < L; l++) {
                    a[n][m][l] = n*M*L+ m* L  + l;
                }
            }
        }
        double start_time1 = omp_get_wtime();
#pragma omp for schedule(static) collapse(3)
        for (int n = 0; n < N; n++) {
            for (int m = 0; m < M; m++) {
                for (int l = 0; l < L; l++) {
#pragma omp critical
                    {
                        shared_value1 += a[n][m][l];
                    }
                }
            }
        }
        double end_time1 = omp_get_wtime();

#pragma omp single
        {
            printf("shared_value1:%d,time:%lf\n",shared_value1,(end_time1 - start_time1));
        }

        start_time1 = omp_get_wtime();
#pragma omp for schedule(static) collapse(3)
        for (int n = 0; n < N; n++) {
            for (int m = 0; m < M; m++) {
                for (int l = 0; l < L; l++) {
#pragma omp atomic update
                    shared_value2 += a[n][m][l];
                }
            }
        }
        end_time1 = omp_get_wtime();
#pragma omp single
        {
            printf("shared_value2:%d,time:%lf\n",shared_value2,(end_time1 - start_time1));
        }

        start_time1 = omp_get_wtime();
#pragma omp for schedule(static) collapse(3) reduction(+:shared_value3)
        for (int n = 0; n < N; n++) {
            for (int m = 0; m < M; m++) {
                for (int l = 0; l < L; l++) {
                    shared_value3 += a[n][m][l];
                }
            }
        }
        end_time1 = omp_get_wtime();
#pragma omp single
        {
            printf("shared_value3:%d,time:%lf\n",shared_value3,(end_time1 - start_time1));
        }
    }
}