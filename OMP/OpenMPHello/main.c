#include <stdio.h>
#include <omp.h>

int main(void) {
    printf("Hello, World!\n");
    #pragma omp parallel num_threads(4)
    {
        printf("ThreadId:%d\n",omp_get_thread_num());
    }
    return 0;
}