//
// Created by Chenx on 2025/10/28.
//
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BACKOFF_TIME 10000
#define MIN_BACKOFF_TIME 10
#define THREAD_NUM 2

typedef struct {
    int back_off_time;
} back_off_lock;

pthread_t threads[THREAD_NUM];
int tids[THREAD_NUM];

int shared_value = 0;
int lock_status = 0;

void init_back_off_lock(back_off_lock* b_lock) {
    b_lock->back_off_time = MIN_BACKOFF_TIME;
}

void b_lock(back_off_lock* b_lock) {
    while (1) {
        while (__sync_add_and_fetch(&lock_status,0) == 1) ;
        while (__sync_val_compare_and_swap(&lock_status,0,1) == 0) {
            b_lock->back_off_time = MIN_BACKOFF_TIME;
            return;
        }
        int time = rand() % b_lock->back_off_time;
        usleep(time);
        if (2 * b_lock->back_off_time <= MAX_BACKOFF_TIME) {
            b_lock->back_off_time = 2 * b_lock->back_off_time;
        } else {
            b_lock->back_off_time = MAX_BACKOFF_TIME;
        }
    }
}

void un_lock(back_off_lock* b_lock) {
    b_lock->back_off_time = MIN_BACKOFF_TIME;
    __sync_lock_release(&lock_status);
}

void* tread_function(void* arg) {
    int thread_id = *(int*) arg;
    srand(time(NULL) ^ (pthread_self() * 1000));
    back_off_lock lock;
    init_back_off_lock(&lock);
    while (1) {
        b_lock(&lock);
        shared_value++;
        printf("thread:%d,shared_value:%d\n",thread_id,shared_value);
        un_lock(&lock);
    }
    return NULL;
}

int main() {
    for (int index = 0; index < THREAD_NUM; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,tread_function,&tids[index]);
    }
    for (int index = 0; index < THREAD_NUM; index++) {
        pthread_join(threads[index],NULL);
    }
}

