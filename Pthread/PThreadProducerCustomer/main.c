#include <stdio.h>
#include <pthread.h>
#define NUMBER_PRODUCERS 10
#define  NUMBER_CONSUMERS 5
pthread_t producers[NUMBER_PRODUCERS];
pthread_t consumers[NUMBER_CONSUMERS];
pthread_mutex_t mutex;
pthread_cond_t cond_producer;
pthread_cond_t cond_consumer;

/**
 * if shared_value == 0, then the consumer waits
 * if shared_value == 1000, then the producer waits
 */
int shared_value = 0;
int producer_ids[NUMBER_PRODUCERS];
int consumer_ids[NUMBER_CONSUMERS];
void init_();
void* producer(void* arg);
void* consumer(void* arg);

int main(void) {
    init_();
    for (int i = 0; i < NUMBER_CONSUMERS; i++)
        pthread_join(consumers[i], NULL);
    return 0;
}

void init_() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_producer, NULL);
    pthread_cond_init(&cond_consumer, NULL);
    for (int i = 0; i < NUMBER_PRODUCERS; i++) {
        producer_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }
    for (int i = 0; i < NUMBER_CONSUMERS; i++) {
        consumer_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }
}

void* producer(void* arg) {
    int thread_id = *(int*) arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        while (shared_value >= 1000) {
            pthread_cond_wait(&cond_producer, &mutex);
        }
        shared_value++;
        printf("Producer %d produced, shared_value = %d\n", thread_id, shared_value);
        pthread_cond_broadcast(&cond_consumer);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    int thread_id = *(int*) arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        while (shared_value <= 0) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }
        shared_value--;
        printf("Consumer %d consumed, shared_value = %d\n", thread_id, shared_value);
        pthread_cond_broadcast(&cond_producer);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}