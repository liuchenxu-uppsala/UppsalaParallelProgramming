#include <stdio.h>
#include "PthreadBarrier.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* workFunction(void * arg);

int main(void) {
    srand((unsigned int)time(NULL));
    initBarrier(10);
    pthread_t threads[10];
    for (int index = 0; index < 10; index++) {
        pthread_create(&threads[index], NULL, workFunction, &index);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("All Threads finished work\n");
    return 0;
}

void* workFunction(void * arg) {
    int threadId = *(int*)arg;
    printf("Thread %d is working\n", threadId);
    sleep(rand() % 10);
    barrier(threadId);
    printf("Thread %d finished work\n", threadId);
    return  NULL;
}