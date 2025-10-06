#include <stdio.h>
#include <omp.h>
#include <math.h>
double calc(double start, double end, double per_length);
int main(void) {
    printf("Hello, World!\n");
    int start =0;
    int end = 10;
    double sum = 0;
    #pragma omp parallel num_threads(32) reduction(+:sum)
    {
        double local_start =  ((double)(end  - start)) * omp_get_thread_num()/ 32;
        double local_end = ((double)(end  - start)) * (omp_get_thread_num() + 1)/ 32;
        printf("ThreadId %d, start:%lf,end:%lf\n",omp_get_thread_num(),local_start,local_end);
        sum = calc(local_start,local_end,0.0000001);
    }
    printf("final sum: %lf\n",sum);
    return 0;
}

double calc(double start, double end, double per_length) {
    double result  = 0;
    for (double i = start; i < end; i += per_length) {
        result += pow(i,2) * per_length;
    }
    return result;
}