//
// Created by Chenx on 2025/10/22.
//
#include <stdio.h>

#include "pthread.h"
#include "PthreadReadWriteLock.h"

#define THREAD_NUMS_READERS 10
#define THREAD_NUMS_WRITERS 10

pthread_t threads_readers[THREAD_NUMS_READERS];
int tids_readers[THREAD_NUMS_READERS];
pthread_t threads_writers[THREAD_NUMS_WRITERS];
int tids_writers[THREAD_NUMS_WRITERS];

int shared_value = 0;

void* function_read(void* arg) {
    int tid = *(int*) arg;
    while (1) {
        read_lock();
        printf("Read thread:%d,read shared value:%d\n",tid,shared_value);
        read_write_unlock();
    }
}

void* function_write(void* arg) {
    int tid = *(int*) arg;
    while (1) {
        write_lock();
        shared_value++;
        printf("Write thread:%d,write shared value:%d\n",tid,shared_value);
        read_write_unlock();
    }
}

int main() {
    init();
    for (int index = 0; index < THREAD_NUMS_READERS; index++) {
        tids_readers[index] = index;
        pthread_create(&threads_readers[index],NULL,function_read,&tids_readers[index]);
    }

    for (int index = 0; index < THREAD_NUMS_WRITERS; index++) {
        tids_writers[index] = index;
        pthread_create(&threads_writers[index],NULL,function_write,&tids_writers[index]);
    }

    for (int index = 0; index < THREAD_NUMS_READERS; index++) {
        pthread_join(threads_readers[index],NULL);
    }
}