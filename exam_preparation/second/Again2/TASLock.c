//
// Created by Chenx on 2025/10/20.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
volatile int lock_status = 0;
volatile int shared_value = 0;
void lock() {
    while (__sync_lock_test_and_set(&lock_status,1));
}
void unlock() {
    __sync_lock_release(&lock_status);
}
pthread_t threads[2];
int tids[2] = {0,1};
#define LOOP_NUM 10000

void* work(void* arg) {
    int tid = *(int*) arg;
    for (int i = 0; i < LOOP_NUM; i++) {
        lock();
        shared_value++;
        printf("Thread:%d,shared_value:%d\n",tid,shared_value);
        unlock();
    }
    return NULL;
}
int main() {
    pthread_create(&threads[0],NULL,work,&tids[0]);
    pthread_create(&threads[1],NULL,work,&tids[1]);
    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    return 0;
}