#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

volatile unsigned char lock_status = 0;
volatile long shared_value = 0;
pthread_t threads[10];
int thread_ids[10];
#define times 500

void lock() {
    while (__sync_lock_test_and_set(&lock_status,1)) {
    }
}

void unlock() {
    __sync_lock_release(&lock_status);
}

void* function(void* arg) {
    int tid = *(int*)arg;
    lock();
    printf("Thread %d: get lock\n", tid);
    for (int index=0; index<times;index++) {
        shared_value ++;
    }
    printf("Thread %d: %d\n", tid, shared_value);
    unlock();
    return NULL;
}

int main(void) {
    printf("Hello, World!\n");
    for (int index = 0; index< 10;index++) {
        thread_ids[index] = index;
        pthread_create(&threads[index], NULL, function, &thread_ids[index]);
    }
    for (int index = 0; index< 10;index++) {
        pthread_join(threads[index], NULL);
    }
    return 0;
}