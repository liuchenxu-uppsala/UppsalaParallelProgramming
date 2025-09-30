#include <pthread.h>
#include "PthreadBarrier.h"

#include <stdio.h>

typedef struct {
    pthread_mutex_t mutex;
    int count;
    pthread_cond_t cond;
} barrier_t;

static barrier_t barrierPthread;
int threadIndex = 0;

void initBarrier(int threadCount) {
    barrierPthread.count = threadCount;
    pthread_cond_init(&barrierPthread.cond, NULL);
    pthread_mutex_init(&barrierPthread.mutex, NULL);
}

void barrier(int threadId) {
    printf("Thread:%d entered barrier\n",threadId);
    pthread_mutex_lock(&barrierPthread.mutex);
    threadIndex++;
    if (barrierPthread.count == threadIndex) {
        pthread_cond_broadcast(&barrierPthread.cond);
    } else {
        pthread_cond_wait(&barrierPthread.cond, &barrierPthread.mutex);
    }
    pthread_mutex_unlock(&barrierPthread.mutex);
}