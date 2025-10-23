//
// Created by Chenx on 2025/10/23.
//
#include <stdio.h>
#include<omp.h>
#define N 31
long fib(int n) {
    long i,j;
    if (n<2)
        return n;
#pragma omp task shared(i) if(n > 30)
    {
        i = fib(n-1);
    }
#pragma omp task shared(j) if(n > 30)
    {
        j = fib(n-2);
    }
#pragma omp taskwait
    return i + j;
}

int main() {
#pragma omp parallel
    {
        int fib_sum = 0;
        double start_time = omp_get_wtime();
#pragma omp single copyprivate(fib_sum)
        {
            fib_sum = fib(N);
        }
        double end_time = omp_get_wtime();
#pragma omp master
        {
            printf("fib_sum:%ld,time:%lf",fib_sum,(end_time - start_time));
        }
    }
}