//
// Created by Chenx on 2025/10/27.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include "PthreadReadWriteLock.h"

#define READER_NUMS 10
#define WRITER_NUMS 1

pthread_t readers[READER_NUMS];
int tids_readers[READER_NUMS];
pthread_t writers[WRITER_NUMS];
int tids_writers[WRITER_NUMS];

int shared_value = 0;

void* read(void* arg) {
    int tid = *(int*) arg;
    while (1) {
        r_lock();
        printf("thread:%d,read shared_value:%d\n",tid,shared_value);
        un_lock();
    }
    return NULL;
}

void* write(void* arg) {
    int tid = *(int*) arg;
    while (1) {
        w_lock();
        shared_value++;
        printf("thread:%d,write shared_value:%d\n",tid,shared_value);
        un_lock();
    }
    return NULL;
}

void init(){
    init_lock();
    for (int index = 0; index < READER_NUMS; index++) {
        tids_readers[index] = index;
        pthread_create(&readers[index],NULL,read,&tids_readers[index]);
    }
    for (int index = 0; index < WRITER_NUMS; index++) {
        tids_writers[index] = index;
        pthread_create(&writers[index],NULL,write,&tids_writers[index]);
    }
    for (int index = 0; index < READER_NUMS; index++) {
        pthread_join(readers[index],NULL);
    }
}

int main() {
    init();
}