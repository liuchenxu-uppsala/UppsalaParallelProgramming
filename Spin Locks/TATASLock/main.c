#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

volatile long shared_value = 0;
volatile unsigned char lock_status = 0;
#define times 500

pthread_t threads[10];
int tids[10];

void lock() {
    while (1) {
        while (__sync_fetch_and_add(&lock_status,0) == 1) {
        }
        if (__sync_val_compare_and_swap(&lock_status,0,1) == 0) {
            return;
        }
    }
}

void unlock() {
    __sync_lock_release(&lock_status);
}

void* function(void* arg) {
    int tid = *(int*) arg;
    lock();
    printf("Thread %d got lock\n", tid);
    for (int i = 0; i < times; i++) {
        shared_value++;
    }
    printf("Thread %d released lock, shared value: %ld\n", tid, shared_value);
    unlock();
    return NULL;
}
int main(void) {
    printf("Hello, World!\n");
    for (int i = 0; i < 10; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, function, &tids[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}