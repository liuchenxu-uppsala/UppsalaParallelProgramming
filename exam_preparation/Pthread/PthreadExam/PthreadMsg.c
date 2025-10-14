//
// Created by Chenx on 2025/10/14.
//
#include <pthread.h>
#include <stdio.h>
#define THREAD_NUMS 20
pthread_t threads[THREAD_NUMS];
int thread_ids[THREAD_NUMS];
int message_slots[THREAD_NUMS];
pthread_mutex_t mutex;
void init();
void* produce_consume(void*);
int main() {
    init();
}

void init() {
    pthread_mutex_init(&mutex,NULL);
    for (int index = 0; index < THREAD_NUMS; index++) {
        thread_ids[index] = index;
        pthread_create(&threads[index],NULL,produce_consume,&thread_ids[index]);
    }
    for (int index = 0; index < THREAD_NUMS; index++) {
        pthread_join(threads[index],NULL);
    }
}

void* produce_consume(void* arg) {
    int thread_id = *(int*)arg;
    int receive_msg_slot = (thread_id - 1 + THREAD_NUMS) % THREAD_NUMS;
    int send_msg_slot = (thread_id + 1) % THREAD_NUMS;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (message_slots[receive_msg_slot]) {
            printf("thread id:%d,consumed message,from slot:%d\n",thread_id,receive_msg_slot);
            message_slots[receive_msg_slot] = 0;
            pthread_mutex_unlock(&mutex);
            continue;
        } else {
            if (!message_slots[send_msg_slot]) {
                printf("thread id:%d,send message,to slot:%d \n",thread_id,send_msg_slot);
                message_slots[send_msg_slot] = 1;
                pthread_mutex_unlock(&mutex);
                continue;
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}