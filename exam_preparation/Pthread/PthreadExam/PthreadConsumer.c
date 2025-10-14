#include <pthread.h>
#include <stdio.h>
#include <bits/pthreadtypes.h>
//
// Created by Chenx on 2025/10/14.
//
pthread_t consumer;
pthread_t producer;
int consume_id = 0;
int produce_id = 1;
pthread_mutex_t mutex;
int message_available;
void* produce_consume(void*);
int main() {
    pthread_mutex_init(&mutex,NULL);
    pthread_create(&consumer,NULL,produce_consume,&consume_id);
    pthread_create(&producer,NULL,produce_consume,&produce_id);
    pthread_join(consumer,NULL);
    pthread_join(producer,NULL);
    return 0;
}


void* produce_consume(void* arg) {
    int my_rank = *(int*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (my_rank == consume_id) {
            if (message_available) {
                printf("thread id:%d,consumed message\n",my_rank);
                message_available = 0;
                pthread_mutex_unlock(&mutex);
                break;
            }
        } else {
            printf("thread id:%d,create message\n",my_rank);
            message_available = 1;
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}