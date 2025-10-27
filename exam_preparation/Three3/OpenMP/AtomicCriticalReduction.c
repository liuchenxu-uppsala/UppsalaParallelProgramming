//
// Created by Chenx on 2025/10/27.
//
#include "omp.h"
#include <stdio.h>
#define NUM 1000000

void calculate(int type){
    if (type == 0) {
        double start_time = omp_get_wtime();
        long long result = 0;
        #pragma omp parallel
        {
            #pragma omp for firstprivate(result) lastprivate(result)
            for (long i = 0; i < NUM; i++) {
                result++;
            }
        }
        double end_time = omp_get_wtime();
        printf("unsafe result:%lld,time:%lf\n",result,(end_time - start_time));
    } else if (type == 1) {
                double start_time = omp_get_wtime();
                long long result = 0;
#pragma omp parallel
                {
                    #pragma omp for
                    for (long i = 0; i < NUM; i++) {
                    #pragma omp critical
                        {
                            result++;
                        }
                    }
#pragma omp single
                    {
                        double end_time = omp_get_wtime();
                        printf("critical result:%lld,time:%lf\n",result,(end_time - start_time));
                    }
                }
            } else if (type == 2){
                double start_time = omp_get_wtime();
                long long result = 0;
#pragma omp parallel
                {
#pragma omp for
                    for (long i = 0; i < NUM; i++) {
#pragma omp atomic update
                        result++;
                    }
#pragma omp single
                    {
                        double end_time = omp_get_wtime();
                        printf("atomic result:%lld,time:%lf\n",result,(end_time - start_time));
                    }
                }
            } else if (type == 3) {
                double start_time = omp_get_wtime();
                long long result = 0;
#pragma omp parallel
                {
#pragma omp for reduction(+:result)
                    for (long i = 0; i < NUM; i++) {
                        result++;
                    }
#pragma omp single
                    {
                        double end_time = omp_get_wtime();
                        printf("reduction result:%lld,time:%lf\n",result,(end_time - start_time));
                    }
                }
            }
}
int main() {
    calculate(0);
    calculate(1);
    calculate(2);
    calculate(3);
}