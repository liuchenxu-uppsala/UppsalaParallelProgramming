#include <stdio.h>
#include<omp.h>
int main(void) {
    printf("Hello, World!\n");
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        for (int index = 0;index<5;index++) {
            printf("Hi ThreadId:%d,index:%d\n",omp_get_thread_num(),index);
        }
    }
}
