//
// Created by Chenx on 2025/10/27.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 1000

unsigned char message[NUM_THREADS];
pthread_t threads[NUM_THREADS];
int tids[NUM_THREADS];
pthread_mutex_t lock;

void* thread_function(void* arg) {
    int thread_id = *(int*) arg;
    while (1) {
        pthread_mutex_lock(&lock);
        if (thread_id + 1 < NUM_THREADS) {
            if (message[thread_id + 1] == 0) {
                message[thread_id + 1] = 1;
                printf("thread_id:%d,send message to %d,value:%d\n",thread_id,thread_id + 1,message[thread_id + 1]);
                pthread_mutex_unlock(&lock);
                continue;
            }
        }
        if (thread_id - 1 >= 0) {
            if (message[thread_id - 1] == 1) {
                message[thread_id - 1] = 0;
                printf("thread_id:%d,consume message from %d,value:%d\n",thread_id,thread_id - 1,message[thread_id - 1]);
                pthread_mutex_unlock(&lock);
                continue;
            }
        }
        pthread_mutex_unlock(&lock);
    }
}

void init() {
    for (int index = 0; index < NUM_THREADS; index++) {
        message[index] = 0;
    }
    for (int index = 0; index < NUM_THREADS; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,thread_function,&tids[index]);
    }
    for (int index = 0; index < NUM_THREADS; index++) {
        pthread_join(threads[index],NULL);
    }
}

int main() {
    init();
}