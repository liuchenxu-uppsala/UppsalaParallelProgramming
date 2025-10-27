//
// Created by Chenx on 2025/10/27.
//
#include "omp.h"
#include <stdio.h>

int main() {
    int shared_value = 0;
#pragma omp parallel private(shared_value)
    {
        shared_value = omp_get_thread_num();
        printf("Before Thread:%d,shared_value:%d\n",omp_get_thread_num(),shared_value);
#pragma omp single copyprivate(shared_value)
        {
            shared_value = omp_get_thread_num();
            printf("Single Thread:%d,shared_value:%d\n",omp_get_thread_num(),shared_value);
            printf("=======================================================================\n");
        }
        printf("After Thread:%d,shared_value:%d\n",omp_get_thread_num(),shared_value);
    }
}