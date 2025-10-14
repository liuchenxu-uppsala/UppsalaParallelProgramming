//
// Created by Chenx on 2025/10/14.
//
#include <omp.h>
#include <stdio.h>
#define N 1000000
void calculate(int type) {
    long long sum = 0;
    omp_set_num_threads(16);
    double start_time = omp_get_wtime();
#pragma omp parallel
    {
#pragma omp for
        for (int index=0;index<N;index++) {
            if (type ==0) {
#pragma omp critical
                {
                    sum++;
                }
            }
            else if (type == 1) {
#pragma omp atomic update
                sum++;
            }
            else {
                sum++;
            }
        }
    }
    double end_time = omp_get_wtime();
    printf("type:%d,time:%lf,sum:%lld\n",type,end_time-start_time,sum);
}

int main() {
    calculate(0);
    calculate(1);
    calculate(2);
    long long reduction_sum = 0;
    omp_set_num_threads(16);
    double start_time = omp_get_wtime();
#pragma omp parallel for reduction(+:reduction_sum)
    for (int index=0;index<N;index++) {
        reduction_sum++;
    }
    double end_time = omp_get_wtime();
    printf("reduction,time:%lf,sum:%lld\n",end_time-start_time,reduction_sum);
}