//
// Created by Chenx on 2025/10/27.
//
#include <stdio.h>
#include<omp.h>
#include "unistd.h"
void task_a() {
    usleep(1000);
}

void task_b() {
    usleep(5000);
}

void task_c() {
    usleep(10000);
}
int main() {
#pragma omp parallel
    {
#pragma omp sections
        {
#pragma omp section
            {
                printf("Thread:%d start task_a\n",omp_get_thread_num());
                task_a();
                printf("Thread:%d end task_a\n",omp_get_thread_num());
            }
#pragma omp section
{
    printf("Thread:%d start task_b\n",omp_get_thread_num());
    task_b();
    printf("Thread:%d end task_b\n",omp_get_thread_num());
}
#pragma omp section
{
    printf("Thread:%d start task_c\n",omp_get_thread_num());
    task_c();
    printf("Thread:%d end task_c\n",omp_get_thread_num());
}
        }
        printf("All task is completed,Thread:%d\n",omp_get_thread_num());
    }
}