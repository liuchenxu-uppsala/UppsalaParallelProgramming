//
// Created by Chenx on 2025/10/14.
//
#include <stdio.h>
#include <stdlib.h>

#include "pthread.h"
typedef struct {
    int thread_count;
    pthread_cond_t barrier_cond;
    pthread_mutex_t mutex;
} barrier_struct;

barrier_struct barrier;
int current_thread_num = 0;

void init_barrier(int thread_nums) {
    barrier.thread_count = thread_nums;
    pthread_cond_init(&barrier.barrier_cond,NULL);
    pthread_mutex_init(&barrier.mutex,NULL);
}

void barrier_pthread(int thread_id) {
    pthread_mutex_lock(&barrier.mutex);
    current_thread_num++;
    printf("thread: %d, arrived barrier, current thread num: %d.\n",thread_id,current_thread_num);
    if (current_thread_num == barrier.thread_count) {
        pthread_cond_broadcast(&barrier.barrier_cond);
    } else {
        pthread_cond_wait(&barrier.barrier_cond,&barrier.mutex);
    }
    pthread_mutex_unlock(&barrier.mutex);
}