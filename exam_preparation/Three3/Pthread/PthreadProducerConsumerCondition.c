//
// Created by Chenx on 2025/10/27.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

#define PRODUCER_NUMS 20
#define CONSUMER_NUMS 10

pthread_t producers[PRODUCER_NUMS];
int producer_ids[PRODUCER_NUMS];
pthread_t consumers[CONSUMER_NUMS];
int consumer_ids[CONSUMER_NUMS];

pthread_cond_t condition_producer;
pthread_cond_t condition_consumer;

pthread_mutex_t lock;

int shared_value = 0;

void* producer(void* arg) {
    int tid = *(int*) arg;
    while (1) {
        pthread_mutex_lock(&lock);
        if (shared_value >= 1000) {
            pthread_cond_wait(&condition_producer,&lock);
        }
        shared_value++;
        printf("producer:%d,shared_value:%d\n",tid,shared_value);
        pthread_cond_broadcast(&condition_consumer);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* consumer(void* arg) {
    int tid = *(int*) arg;
    while (1) {
        pthread_mutex_lock(&lock);
        if (shared_value <= 0) {
            pthread_cond_wait(&condition_consumer,&lock);
        }
        shared_value--;
        printf("consumer:%d,shared_value:%d\n",tid,shared_value);
        pthread_cond_broadcast(&condition_producer);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void init() {
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&condition_producer,NULL);
    pthread_cond_init(&condition_consumer,NULL);
    for (int index = 0; index < PRODUCER_NUMS; index++) {
        producer_ids[index] = index;
        pthread_create(&producers[index],NULL,producer,&producer_ids[index]);
    }
    for (int index = 0; index < CONSUMER_NUMS; index++) {
        consumer_ids[index] = index;
        pthread_create(&consumers[index],NULL,consumer,&consumer_ids[index]);
    }
    for (int index = 0; index < PRODUCER_NUMS; index++) {
        pthread_join(producers[index],NULL);
    }
}

int main() {
    init();
}