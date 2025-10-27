//
// Created by Chenx on 2025/10/27.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    int thread_num;
    pthread_cond_t barrier_condition;
    pthread_mutex_t lock;
} Pthread_barrier;

int current_thread_nums;
Pthread_barrier p_barrier;

void init_barrier(int num) {
    p_barrier.thread_num = num;
    pthread_mutex_init(&p_barrier.lock,NULL);
    pthread_cond_init(&p_barrier.barrier_condition,NULL);
}

void barrier(int tid) {
    pthread_mutex_lock(&p_barrier.lock);
    current_thread_nums++;
    printf("Thread:%d arrived,current_thread_nums:%d\n",tid,current_thread_nums);
    if (current_thread_nums < p_barrier.thread_num) {
        pthread_cond_wait(&p_barrier.barrier_condition,&p_barrier.lock);
    }
    pthread_cond_broadcast(&p_barrier.barrier_condition);
    pthread_mutex_unlock(&p_barrier.lock);
}