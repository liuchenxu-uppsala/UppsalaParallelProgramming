//
// Created by Chenx on 2025/10/27.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "PThreadBarrier.h"

#define THREAD_NUM 10
pthread_t threads[THREAD_NUM];
int thread_ids[THREAD_NUM];

void* thread_function(void* arg) {
    int thread_id = *(int*) arg;
    sleep(rand() % 10);
    barrier(thread_id);
    printf("Thread:%d finished\n",thread_id);
    return NULL;
}
int main() {
    init_barrier(THREAD_NUM);
    srand((unsigned int)time(NULL));
    for (int index = 0; index < THREAD_NUM; index++) {
        thread_ids[index] = index;
        pthread_create(&threads[index],NULL,thread_function,&thread_ids[index]);
    }
    for (int index = 0; index < THREAD_NUM; index++) {
        pthread_join(threads[index],NULL);
    }
}