//
// Created by Chenx on 2025/10/15.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>
#define N 1024
int* input;
int* output;
void parallel_prefix_sum(int* input, int* output, int n);
int main() {
    input = (int*)malloc(sizeof(int) * N);
    output = (int*)malloc(sizeof(int) * N);
    for (int i = 0; i < N; i++) {
        input[i] = i;
    }
    parallel_prefix_sum(input,output,N);
    for (int i = 0; i < N; i++) {
        printf("output[%d]:%d\n",i,output[i]);
    }
    return 0;
}
void parallel_prefix_sum(int* input, int* output, int n) {
    int num_threads;
    int* block_sum;
    int block_size;
    #pragma omp parallel
    {
        #pragma omp single
        {
            num_threads = omp_get_num_threads();
            block_sum = (int*)malloc(sizeof(int) * num_threads);
            block_size = n / num_threads;
        }
        int tid = omp_get_thread_num();
        int start = tid * block_size;
        int end;
        if (tid == num_threads - 1) {
            end  = n;
        } else {
            end = start + block_size;
        }
        output[start] = input[start];
        for (int i = start + 1; i < end; i++) {
            output[i] = input[i] + output[i-1];
        }
        block_sum[tid] = output[end-1];
        #pragma omp barrier
        int offset = 0;
        for (int i = 0; i< tid;i++) {
            offset = offset + block_sum[i];
        }
        for (int index=start;index<end;index++) {
            output[index] = output[index] + offset;
        }
    }
}