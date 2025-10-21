//
// Created by Chenx on 2025/10/21.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#define sample_points  1000000000
#define thread_nums  10
pthread_t threads[thread_nums];
long nums[thread_nums];
long samples_per_thread;
long final_result = 0;

void* compute_pi(void* arg) {
    long* local_result = (long*)arg;
    long temp_result = 0;
    unsigned int seed = (unsigned int)time(NULL) ^ (pthread_self());
    for (int index = 0; index < samples_per_thread; index++) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double distance = x*x +y*y;
        if (distance <= 1) {
            temp_result++;
        }
    }
    *local_result = temp_result;
}
int main() {
    srand((unsigned int)time(NULL));
    samples_per_thread = sample_points / thread_nums;
    for (int index = 0; index < thread_nums; index++) {
        nums[index] = 0;
        pthread_create(&threads[index],NULL,compute_pi,&nums[index]);
    }
    for (int index = 0; index < thread_nums; index++) {
        pthread_join(threads[index],NULL);
    }
    for (int index = 0; index < thread_nums; index++) {
        final_result += nums[index];
    }
    printf("Pi value:%lf\n",(4.0 * final_result)/sample_points);
}
