//
// Created by Chenx on 2025/10/14.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
long fib(int n);
#define N 40
int main() {
    double start_time = omp_get_wtime();
    long value = 0;
#pragma omp parallel
    {
#pragma omp single
        {
            value = fib(N);
        }
    }
    double end_time = omp_get_wtime();
    printf("fib(%d):%ld,time:%f",N,value,end_time - start_time);
    return 0;
}

long fib(int n) {
    long i,j;
    if (n<2)
        return n;
#pragma omp task shared(i) if(n>30)
    {
i = fib(n-1);
    }
#pragma omp task shared(j) if(n>30)
    {
        j = fib(n-2);
    }
#pragma omp taskwait
    return i + j;
}