//
// Created by Chenx on 2025/10/21.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

#define THREAD_NUMS 20
#define consumer_id 0
#define produce_id 1
int shared_value;
pthread_mutex_t mutex;
pthread_t threads[THREAD_NUMS];
int tids[THREAD_NUMS];

void* produce_consume(void* arg) {
    int tid = *(int*) arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        // 生产者
        if (tid % 2 == 0) {
            if (shared_value < 1000) {
                shared_value++;
                printf("Thread:%d,produced shared_value:%d\n",tid,shared_value);
                pthread_mutex_unlock(&mutex);
                continue;
            }
        } else if (tid % 2 == 1) { //消费者
            if (shared_value > 0) {
                shared_value--;
                printf("Thread:%d,consumed shared_value:%d\n",tid,shared_value);
                pthread_mutex_unlock(&mutex);
                continue;
            }
        }
        pthread_mutex_unlock(&mutex);
    }
}

void init() {
    for (int index = 0; index < THREAD_NUMS; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,produce_consume,&tids[index]);
    }
    for (int index = 0; index < THREAD_NUMS; index++) {
        pthread_join(threads[index],NULL);
    }
}

int main() {
    init();
}