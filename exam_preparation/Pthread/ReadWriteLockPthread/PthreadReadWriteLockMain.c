//
// Created by Chenx on 2025/10/14.
//
#include <stdio.h>

#include "PthreadReadWriteLock.h"
#include "pthread.h"
#define READ_NUMS 5
#define WRITE_NUMS 2
pthread_t readers[READ_NUMS];
pthread_t writers[WRITE_NUMS];
int reader_ids[READ_NUMS];
int writers_ids[WRITE_NUMS];
void* read_function(void*);
void* write_function(void*);
int shared_value = 0;
int main() {
    init_reader_writer_lock();
    for (int i = 0; i < READ_NUMS; i++) {
        reader_ids[i] = i;
        pthread_create(&readers[i],NULL,read_function,&reader_ids[i]);
    }
    for (int i = 0; i < WRITE_NUMS; i++) {
        writers_ids[i] = i;
        pthread_create(&writers[i],NULL,write_function,&writers_ids[i]);
    }

    for (int index = 0; index < READ_NUMS; index++) {
        pthread_join(readers[index],NULL);
    }

    for (int index = 0; index < WRITE_NUMS; index++) {
        pthread_join(writers[index],NULL);
    }
}

void* read_function(void* arg) {
    int thread_id = *(int*)arg;
    while (shared_value<11) {
        read_lock();
        printf("thread id:%d,reading. shared_value:%d\n",thread_id,shared_value);
        read_write_unlock();
    }
    return NULL;
}

void* write_function(void* arg) {
    int thread_id = *(int*)arg;
    while (shared_value<11) {
        write_lock();
        shared_value++;
        printf("thread id:%d,writing. shared_value:%d\n",thread_id,shared_value);
        read_write_unlock();
    }
    return NULL;
}