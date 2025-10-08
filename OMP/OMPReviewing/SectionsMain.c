//
// Created by Chenx on 2025/10/8.
//
#include <stdio.h>
#include<omp.h>
int main() {
    int shared_value = 100;
#pragma omp parallel
    {
        #pragma omp sections private(shared_value)
        {
            #pragma omp section
            {
                printf("Section1 ThreadId:%d\n",omp_get_thread_num());
                shared_value = 1;
                printf("section1 shared value:%d\n",shared_value);
            }
#pragma omp section
{
    printf("Section2 ThreadId:%d\n",omp_get_thread_num());
    shared_value = 2;
    printf("section2 shared value:%d\n",shared_value);
}
#pragma omp section
{
    printf("Section3 ThreadId:%d\n",omp_get_thread_num());
    shared_value = 3;
    printf("section3 shared value:%d\n",shared_value);
}
#pragma omp section
{
    printf("Section4 ThreadId:%d\n",omp_get_thread_num());
    shared_value = 4;
    printf("section4 shared value:%d\n",shared_value);
}
        }
        printf("Thread %d: **已通过 sections 后的屏障，继续执行**\n", omp_get_thread_num());

    }
    printf("final shared value:%d\n",shared_value);

    #pragma omp parallel for firstprivate(shared_value)
    for (int i = 0; i < 32; i++) {
        shared_value++;
        printf("ThreadId:%d,shared_value:%d\n",omp_get_thread_num(),shared_value);
    }
    printf("first private shared value:%d\n",shared_value);

#pragma omp parallel
    {
#pragma omp for lastprivate(shared_value) firstprivate(shared_value)
        for (int i = 0; i < 32; i++) {
            shared_value++;
        }
    }
    printf("last private shared value:%d\n",shared_value);
    return 0;
}