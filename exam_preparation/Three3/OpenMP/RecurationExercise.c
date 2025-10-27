//
// Created by Chenx on 2025/10/27.
//
#include "omp.h"
#include <stdio.h>
#define NUM 1000
int A[NUM];
long long sum(int start,int end) {
    if (end - start < 1000) {
        long long sum = 0;
        for (int index = start; index <= end; index++) {
            sum = sum + A[index];
        }
        return sum;
    } else {
        long long left = 0;
        long long right = 0;
#pragma omp task shared(left)
        {
            left = sum(start,(end + start) / 2);
        }
#pragma omp task shared(right)
        {
            right = sum((end + start) / 2 + 1, end);
        }
#pragma omp taskwait
        return left + right;
    }
}

int main() {
#pragma omp parallel
    {
#pragma omp for
        for (int index = 0; index < NUM; index++) {
            A[index] = index;
        }
#pragma omp single
        {
            long long result = sum(0,NUM -1);
            printf("result:%lld\n",result);
        }
    }
}