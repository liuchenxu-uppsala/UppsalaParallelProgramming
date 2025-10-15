//
// Created by Chenx on 2025/10/15.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
#define N 1000000000
long long arr[N];
int main() {
    arr[0] = 0;
#pragma omp parallel for schedule(dynamic)
    for (int index = 1;index<N;index++) {
        arr[index] = (long long) (index+1) * index/2;
    }
    for (int index = 0;index<N;index++) {
        printf("arr[%d]:%lld\n",index,arr[index]);
    }
}