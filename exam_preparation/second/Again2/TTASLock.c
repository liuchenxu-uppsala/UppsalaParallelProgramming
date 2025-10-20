//
// Created by Chenx on 2025/10/20.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#define LOOP_NUM 10000
volatile int lock_status = 0;
volatile int shared_value = 0;
pthread_t threads[2];
int tids[2] = {0,1};
void lock() {
    while (1) {
        while (__sync_add_and_fetch(&lock_status,0) == 1) {}
        while (__sync_val_compare_and_swap(&lock_status,0,1) == 0) {
            return;
        }
    }
}

void unlock() {
    __sync_lock_release(&lock_status);
}

void* work(void* arg) {
    int tid = *(int*) arg;
    for (int i = 0; i < LOOP_NUM; i++) {
        lock();
        shared_value++;
        printf("Thread:%d,shared_value:%d\n",tid,shared_value);
        unlock();
    }
}
int main() {
    pthread_create(&threads[0],NULL,work,&tids[0]);
    pthread_create(&threads[1],NULL,work,&tids[1]);
    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    printf("shared_value:%d\n",shared_value);
    return 0;
}