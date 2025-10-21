//
// Created by Chenx on 2025/10/21.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

#define THREADS_NUM 10
pthread_t threads[THREADS_NUM];
int message_slots[THREADS_NUM];
int tids[THREADS_NUM];
pthread_mutex_t mutex;

void* work(void* arg) {
    int tid = *(int*) arg;
    int send_message_slot = tid + 1;
    int receive_message = tid - 1;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (message_slots[send_message_slot] == 0) {
            printf("Thread:%d,send message to %d\n",tid,send_message_slot);
            message_slots[send_message_slot] = 1;
            pthread_mutex_unlock(&mutex);
            continue;
        } else if (message_slots[receive_message] == 1) {
            printf("Thread:%d,receive message from %d\n",tid,receive_message);
            message_slots[receive_message] = 0;
            pthread_mutex_unlock(&mutex);
            continue;
        }
        pthread_mutex_unlock(&mutex);
    }
}

void init() {
    for (int index = 0; index < THREADS_NUM; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,work,&tids[index]);
    }
    for (int index = 0; index < THREADS_NUM; index++) {
        pthread_join(threads[index],NULL);
    }
}

int main() {
    init();
}