//
// Created by Chenx on 2025/10/21.
//
#include <stdio.h>

#include "pthread.h"
typedef struct {
    int thread_nums;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
} p_barrier_t;

p_barrier_t barrier;
int current_threads = 0;

void init_barrier(int t_num) {
    pthread_mutex_init(&barrier.mutex,NULL);
    pthread_cond_init(&barrier.condition,NULL);
    barrier.thread_nums = t_num;
}

void barrier_p(int threadId) {
    pthread_mutex_lock(&barrier.mutex);
    current_threads++;
    printf("Thread:%d arrived,current_threads:%d\n",threadId,current_threads);
    while (current_threads < barrier.thread_nums) {
        pthread_cond_wait(&barrier.condition,&barrier.mutex);
    }
    printf("Thread:%d,left\n",threadId);
    pthread_cond_broadcast(&barrier.condition);
    pthread_mutex_unlock(&barrier.mutex);
}