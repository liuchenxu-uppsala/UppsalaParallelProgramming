//
// Created by Chenx on 2025/10/28.
//
#include <pthread.h>
#include <stdio.h>
int lock_status = 0;
int shared_value = 0;
#define THREAD_NUM 2
pthread_t threads[THREAD_NUM];
int tids[THREAD_NUM];

void lock() {
    while (1) {
        while (__sync_add_and_fetch(&lock_status,0) == 1);
        while (__sync_val_compare_and_swap(&lock_status,0,1) == 0) {
            return;
        }
    }
}

void unlock() {
    __sync_lock_release(&lock_status);
}

void* thread_function(void* arg) {
    int tid = *(int*)arg;
    while (1) {
        lock();
        shared_value++;
        printf("Thread:%d,shared_value:%d\n",tid,shared_value);
        unlock();
    }
}

int main() {
    for (int index = 0; index < THREAD_NUM; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,thread_function,&tids[index]);
    }
    for (int index = 0; index < THREAD_NUM; index++) {
        pthread_join(threads[index],NULL);
    }
}