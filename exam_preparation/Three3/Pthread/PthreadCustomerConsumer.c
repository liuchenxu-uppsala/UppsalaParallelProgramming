//
// Created by Chenx on 2025/10/27.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#define THREADS_NUM 100
pthread_t threads[THREADS_NUM];
int tids[THREADS_NUM];
pthread_mutex_t lock;
long shared_value = 0;

void* thread_function(void* arg) {
    int thread_id = *(int*) arg;
    bool is_customer = thread_id % 2;
    while (1) {
        pthread_mutex_lock(&lock);
        if (is_customer) {
            if (shared_value <= 0) {
                pthread_mutex_unlock(&lock);
            } else {
                shared_value--;
                printf("Customer:%d,shared_value:%ld\n",thread_id,shared_value);
                pthread_mutex_unlock(&lock);
            }
        } else {
            if (shared_value >= 1000) {
                pthread_mutex_unlock(&lock);
            } else {
                shared_value++;
                printf("Producer:%d,shared_value:%ld\n",thread_id,shared_value);
                pthread_mutex_unlock(&lock);
            }
        }
    }
    return NULL;
}

int main() {
    for (int index = 0; index < THREADS_NUM; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,thread_function,&tids[index]);
    }
    for (int index = 0; index < THREADS_NUM; index++) {
        pthread_join(threads[index],NULL);
    }
}