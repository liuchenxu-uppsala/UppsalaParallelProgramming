//
// Created by Chenx on 2025/10/23.
//
#include <stdio.h>
#include<omp.h>
long long sum(int start, int end) {
    long long left = 0;
    long long right = 0;
    long long sum = 0;
    if ((end - start) > 10000) {
#pragma omp task shared(left)
        {
            left = sum(start,(start + end) / 2);
        }
#pragma omp task shared(right)
        {
            right = sum((start + end) / 2 + 1 ,end);
        }
#pragma omp taskwait
        return left + right;
    }  else {
        for (int index = 0; index <= end; index++) {
            sum+= index * index;
        }
        return sum;
    }
}