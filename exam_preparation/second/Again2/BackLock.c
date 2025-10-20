//
// Created by Chenx on 2025/10/20.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h> // For rand(), srand()
#include <unistd.h> // For usleep()
#include <time.h>   // For time()
#include <math.h>   // For fmin (需要链接 -lm)
#define LOOP_NUM 10000
#define MIN_DELAY 100
#define MAX_DELAY 10000
volatile int lock_status = 0;
volatile int shared_value = 0;
typedef struct {
    int delay;
} back_off_t;

void init_lock(back_off_t* lock_t) {
    lock_t->delay = MIN_DELAY;
}

void back_off(back_off_t* lock_t) {
    int time = rand() % lock_t->delay;
    usleep(time);
    lock_t->delay = fmin(lock_t->delay*2,MAX_DELAY);
}

void back_off_lock(back_off_t* lock_t) {
    while (1) {
        while (__sync_fetch_and_add(&lock_status,0) == 1){}
        if (__sync_val_compare_and_swap(&lock_status,0,1) == 0) {
            lock_t->delay = MIN_DELAY;
            return;
        }
        back_off(lock_t);
    }
}

void unlock_back_off(back_off_t* lock_t) {
    lock_t->delay = MIN_DELAY;
    __sync_lock_release(&lock_status);
}

pthread_t threads[2];
int tids[2] = {0,1};

void* work(void* arg) {
    int tid = *(int*)arg;
    srand(time(NULL) ^ (pthread_self() * 1000));
    back_off_t lock;
    init_lock(&lock);
    for (int i = 0; i < LOOP_NUM; i++) {
        back_off_lock(&lock);
        shared_value++;
        printf("Thread:%d,shared_value:%d\n",tid,shared_value);
        unlock_back_off(&lock);
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