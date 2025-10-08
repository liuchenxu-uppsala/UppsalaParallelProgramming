//
// Created by Chenx on 2025/10/8.
//
#include <stdio.h>
#include<omp.h>
int main() {
    omp_set_num_threads(4);
    #pragma omp parallel for collapse(2) schedule(static)
    for (int index = 0;index < 10; index++) {
        // printf("Hi ThreadId:%d,index:%d\n",omp_get_thread_num(),index);
        for (int j = 0;j < 10; j++) {
            printf("Hi index:%d,ThreadId:%d,j:%d\n",index,omp_get_thread_num(),j);
        }
    }
    return 0;
}