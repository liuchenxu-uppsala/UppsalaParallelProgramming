//
// Created by Chenx on 2025/10/27.
//
#include "omp.h"
#include <stdio.h>
#define NUM 1000
long long fib(int n) {
    long long i,j;
    if (n<2)
        return n;
#pragma omp task shared(i)  if(n > 30)
    i = fib(n-1);
#pragma omp task shared(j) if(n > 30)
    j = fib(n-2);
#pragma omp taskwait
    return i + j;
}

int main() {
    long long result = 0;
#pragma omp parallel shared(result)
    {
        double start = omp_get_wtime();
#pragma omp single
        {
            result = fib(NUM);
        }
#pragma omp single
        {
            double end_time = omp_get_wtime();
            printf("result:%lld,time:%lf\n",result,(end_time - start));
        }
    }
}