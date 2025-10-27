//
// Created by Chenx on 2025/10/27.
//
#include "omp.h"
#include <stdio.h>

int global_value = 1000;
#pragma omp threadprivate(global_value)

int main() {
    global_value = 1001;
#pragma omp parallel copyin(global_value)
    {
        printf("thread:%d,global_value:%d\n",omp_get_thread_num(),global_value);
    }

}