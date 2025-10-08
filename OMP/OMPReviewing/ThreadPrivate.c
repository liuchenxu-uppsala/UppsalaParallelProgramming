#include <omp.h>
#include <stdio.h>
//
// Created by Chenx on 2025/10/8.
//
int value = 100;
#pragma omp threadprivate(value)
int value_2 = 100;
#pragma omp threadprivate(value_2)
int main() {
    value_2 = 99;
#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < 6; i++) {
            value = value + omp_get_thread_num();
        }
    }

#pragma omp parallel copyin(value_2)
    {
        //printf("ThreadId:%d,value:%d\n",omp_get_thread_num(),value);
        printf("ThreadId:%d,value_2:%d\n",omp_get_thread_num(),value_2);
        int copyprivate_value = 100;
        #pragma omp single copyprivate(copyprivate_value)
        {
            copyprivate_value = 7;
        }
        printf("ThreadId:%d,copyprivate_value:%d\n",omp_get_thread_num(),copyprivate_value);
    }
    int sum = 20;
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < 16; i++) {
        sum = omp_get_thread_num();
        printf("ThreadId:%d,local sum:%d\n",omp_get_thread_num(),sum);
    }
    printf("sum:%d\n",sum);
    return 0;
}