//
// Created by Chenx on 2025/10/14.
//
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#define NUM_PRODUCER 10
#define NUM_CONSUMER 5
int shared_value = 0;
pthread_mutex_t mutex;
pthread_t producers[NUM_PRODUCER];
pthread_t consumers[NUM_CONSUMER];
int producer_ids[NUM_PRODUCER];
int consumer_ids[NUM_CONSUMER];
pthread_cond_t cond_producer;
pthread_cond_t cond_consumer;

void init();
void* produce(void*);
void* consume(void*);

int main() {
    init();
    for (int index = 0; index<NUM_CONSUMER; index++) {
        pthread_join(consumers[index],NULL);
    }
}

void init() {
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond_producer,NULL);
    pthread_cond_init(&cond_consumer,NULL);
    for (int index=0; index<NUM_PRODUCER;index++) {
        producer_ids[index] = index;
        pthread_create(&producers[index],NULL,produce,&producer_ids[index]);
    }
    for (int index=0; index<NUM_CONSUMER;index++) {
        consumer_ids[index] = index;
        pthread_create(&consumers[index],NULL,consume,&consumer_ids[index]);
    }
}

void* produce(void* arg) {
    int thread_id = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (shared_value >= 1000) {
            pthread_cond_wait(&cond_producer,&mutex);
        }
        shared_value++;
        printf("thread id:%d,produce value:%d\n",thread_id,shared_value);
        pthread_cond_broadcast(&cond_consumer);
        pthread_mutex_unlock(&mutex);
    }
}

void* consume(void* arg) {
    int thread_id = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (shared_value <= 0 ) {
            pthread_cond_wait(&mutex,&cond_consumer);
        }
        printf("thread id:%d,consume value:%d\n",thread_id,shared_value);
        shared_value--;
        pthread_cond_broadcast(&cond_producer);
        pthread_mutex_unlock(&mutex);
    }
}