//
// Created by Chenx on 2025/10/23.
//
#include <stdio.h>
#include<omp.h>
long sum_range(int start, int end) {
    long sum = 0;
    if (start == end) return start;
    if (end - start > 1000) {
        long left = 0;
        long right = 0;
#pragma omp task shared(left)
        {
            left = sum_range(start,(end + start)/2);
        }
#pragma omp task shared(right)
{
    right = sum_range((end + start)/2 + 1,end);
}
#pragma omp taskwait
        return left + right;
    } else {
        for (int index = start; index <= end; index++) {
            sum += index;
        }
        return sum;
    }
}

int main() {
#pragma omp parallel
    {
#pragma omp single
        {
            long value = sum_range(0, 2000);
            printf("value:%ld\n",value);
        }
    }
}