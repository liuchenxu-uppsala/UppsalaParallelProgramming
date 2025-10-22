//
// Created by Chenx on 2025/10/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "PthreadBarrier.h"
#include "pthread.h"

#define THREAD_NUMS 10
pthread_t threads[THREAD_NUMS];
int tids[THREAD_NUMS];

void* work(void* arg) {
    int tid = *(int*) arg;
    printf("Thread:%d start to work\n",tid);
    sleep(rand() % 10);
    barrier_p(tid);
    printf("Thread:%d stopped the work\n",tid);
    return NULL;
}
int main() {
    init_barrier(THREAD_NUMS);
    srand((unsigned int)time(NULL));
    for (int index = 0; index < THREAD_NUMS; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,work,&tids[index]);
    }
    for (int index = 0; index < THREAD_NUMS; index++) {
        pthread_join(threads[index],NULL);
    }
}