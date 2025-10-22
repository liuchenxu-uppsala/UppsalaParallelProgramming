//
// Created by Chenx on 2025/10/21.
//
#include <stdio.h>
#include "pthread.h"
#define NUM_PRODUCERS 10
#define NUM_CONSUMERS 5
pthread_t producers[NUM_PRODUCERS];
pthread_t consumers[NUM_CONSUMERS];
int tids_produce[NUM_PRODUCERS];
int tids_consume[NUM_CONSUMERS];

pthread_mutex_t mutex;
pthread_cond_t producer_condtion;
pthread_cond_t consumer_condition;

int shared_value = 0;

void* produce(void* arg) {
    int tid = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        while (shared_value >= 1000) {
            pthread_cond_wait(&producer_condtion,&mutex);
        }
        shared_value++;
        printf("Thread:%d,produced,shared_value:%d\n",tid,shared_value);
        pthread_cond_broadcast(&consumer_condition);
        pthread_mutex_unlock(&mutex);
    }
}

void* consume(void* arg) {
    int tid = *(int*) arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        while (shared_value <= 0) {
            pthread_cond_wait(&consumer_condition,&mutex);
        }
        shared_value--;
        printf("Thread:%d,consumed,shared_value:%d\n",tid,shared_value);
        pthread_cond_broadcast(&producer_condtion);
        pthread_mutex_unlock(&mutex);
    }
}

void init() {
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&producer_condtion,NULL);
    pthread_cond_init(&consumer_condition,NULL);
    for (int index = 0; index < NUM_PRODUCERS; index++) {
        tids_produce[index] = index;
        pthread_create(&producers[index],NULL,produce,&tids_produce[index]);
    }
    for (int index = 0; index < NUM_CONSUMERS; index++) {
        tids_consume[index] = index;
        pthread_create(&consumers[index],NULL,consume,&tids_consume[index]);
    }
    for (int index = 0; index < NUM_PRODUCERS; index++) {
        pthread_join(producers[index],NULL);
    }
    for (int index = 0; index < NUM_CONSUMERS; index++) {
        pthread_join(consumers[index],NULL);
    }
}

int main() {
    init();
}