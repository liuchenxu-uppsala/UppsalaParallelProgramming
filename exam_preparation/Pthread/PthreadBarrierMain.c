//
// Created by Chenx on 2025/10/14.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "PthreadBarrier.h"
#include <unistd.h>
#define THREAD_NUMS 10
void* work(void*);
pthread_t threads[THREAD_NUMS];
int thread_ids[THREAD_NUMS];
int main() {
    srand((unsigned int)time(NULL));
    init_barrier(THREAD_NUMS);
    for (int index = 0; index<THREAD_NUMS;index++) {
        thread_ids[index] = index;
        pthread_create(&threads[index],NULL,work,&thread_ids[index]);
    }
    //printf("All threads cross the barrier\n");
    for (int index = 0; index<THREAD_NUMS;index++) {
        pthread_join(threads[index],NULL);
    }
    return 0;
}

void* work(void* arg) {
    int thread_id = *(int*) arg;
    printf("thread:%d start to work\n",thread_id);
    sleep(rand() % 10);
    barrier_pthread(thread_id);
    printf("thread:%d finished work\n",thread_id);
    return NULL;
}