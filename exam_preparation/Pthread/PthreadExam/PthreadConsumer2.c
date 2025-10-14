#include <pthread.h>
#include <stdio.h>
#include <bits/pthreadtypes.h>
//
// Created by Chenx on 2025/10/14.
// 奇数是消费者
// 偶数是生产者
//
#define THREAD_NUMS 2000
pthread_t consumers[THREAD_NUMS/2];
pthread_t producers[THREAD_NUMS/2];
int thread_ids[THREAD_NUMS];
int consume_id = 1;
int produce_id = 0;
pthread_mutex_t mutex;
int message_available;
void* produce_consume(void*);
int main() {
    pthread_mutex_init(&mutex,NULL);

    for (int index = 0; index < THREAD_NUMS; index++) {
        thread_ids[index] = index;
        if (index % 2 == 1) {
            pthread_create(&consumers[index/2],NULL,produce_consume,&thread_ids[index]);
        } else {
            pthread_create(&producers[index/2],NULL,produce_consume,&thread_ids[index]);
        }
    }

    for (int index = 0; index < THREAD_NUMS/2; index++) {
        pthread_join(consumers[index],NULL);
        pthread_join(producers[index],NULL);
    }
    return 0;
}


void* produce_consume(void* arg) {
    int thread_id = *(int*)arg;
    int my_rank = thread_id % 2;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (my_rank == consume_id) {
            if (message_available) {
                printf("thread id:%d,consumed message\n",thread_id);
                message_available = 0;
                pthread_mutex_unlock(&mutex);
                break;
            }
        } else {
            if (!message_available) {
                printf("thread id:%d,create message\n",thread_id);
                message_available = 1;
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}