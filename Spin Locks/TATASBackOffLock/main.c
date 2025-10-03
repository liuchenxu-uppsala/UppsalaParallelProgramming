#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h> // For rand(), srand()
#include <unistd.h> // For usleep()
#include <time.h>   // For time()
#include <math.h>   // For fmin (需要链接 -lm)

#define MAXDELAY 10000
#define MINDELAY 10
#define TIMES 500
#define min(a, b) (((a) < (b)) ? (a) : (b))

volatile unsigned char lock_status = 0;
volatile unsigned long shared_value = 0;
pthread_t threads[10];
int pids[10];

typedef struct {
    int limit;
} BackOff;

void initDelay(BackOff* delay) {
    delay->limit = MINDELAY;
}
void delay(BackOff* delay) {
    int sleep = rand() % delay->limit;
    usleep(sleep);
    delay->limit = min(2*delay->limit,MAXDELAY);
}
void lock(BackOff* backOff) {
    while (1) {
        while (__sync_fetch_and_add(&lock_status,0) == 1){}
        if (__sync_val_compare_and_swap(&lock_status,0,1) == 0) {
            backOff->limit = MINDELAY;
            return;
        }
        delay(backOff);
    }
}

void unlock() {
    __sync_lock_release(&lock_status);
}

void* function(void* arg) {
    int id = *(int*) arg;
    srand(time(NULL) ^ (pthread_self() * 1000));
    BackOff off;
    initDelay(&off);
    lock(&off);
    printf("Thread %d: get lock\n", id);
    for (int i = 0; i < TIMES; i++) {
        shared_value++;
    }
    printf("Thread %d: release lock,shared_value:%ld\n", id, shared_value);
    unlock();
    return NULL;
}

int main(void) {
    printf("Hello, World!\n");
    for (int i = 0; i < 10; i++) {
        pids[i] = i;
        pthread_create(&threads[i], NULL, function, &pids[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i],NULL);
    }
    return 0;
}