//
// Created by Chenx on 2025/10/14.
//
#include <stdio.h>
#include <omp.h>
#define SIZE 10
int arr[SIZE][SIZE];
void init_array();
int main(void) {
    printf("Hello, World!\n");
    init_array();
    return 0;
}
void init_array() {
#pragma omp parallel
    {
#pragma omp for collapse(2) schedule(dynamic, 5) //ordered
        for (int i = 0;i<SIZE;i++) {
            for (int j = 0;j<SIZE;j++) {
                arr[i][j] = omp_get_thread_num();
// #pragma omp ordered
//                 {
                    printf("arr[%d][%d]:%d\n",i,j,arr[i][j]);
                // }
            }
        }
    }
}