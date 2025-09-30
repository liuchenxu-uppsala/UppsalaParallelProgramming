#include <stdio.h>
#include "ReadWriteLock.h"
#include <pthread.h>
long shared_counter = 0;

/**
 * simulate readers
 * @param arg
 * @return
 */
void* readFunction(void * arg) {
    int threadId = *(int*)arg;
    for (int index = 0; index < 3; index++) {
        rLock();
        printf("Thread %d read %ld\n", threadId, shared_counter);
        unlockRWLock();
    }
    return NULL;
}

/**
 * simulate the writers action
 * @param arg
 * @return
 */
void* writeFunction(void * arg) {
    int threadId = *(int*)arg;
    for (int index = 0; index < 3; index++) {
        rLock();
        long oldValue = shared_counter;
        shared_counter++;
        printf("Thread %d write oldValue: %ld, new value: %ld\n", threadId, oldValue, shared_counter);
        unlockRWLock();
    }
    return NULL;
}
#define  NUM_READERS 3
#define NUM_WRITERS 2

pthread_t readers[NUM_READERS];
pthread_t writers[NUM_WRITERS];

int main(void) {
    printf("--- ReadWriteLock Demo start ---\n");
    readWriteLockInit();
    for (int index = 0; index < NUM_READERS; index++) {
        pthread_create(&readers[index], NULL, readFunction, &index);
    }
    for (int index = 0; index < NUM_WRITERS; index++) {
        pthread_create(&writers[index], NULL, writeFunction, &index);
    }
    // join for  all threads
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    printf("--- ReadWriteLock Demo end and final value: %ld ---\n", shared_counter);
    return 0;
}