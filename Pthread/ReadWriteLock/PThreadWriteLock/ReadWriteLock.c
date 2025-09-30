#include <pthread.h>
#include "ReadWriteLock.h"
typedef struct {
    int readers;
    int writers;
    int pendingWriters;
    pthread_mutex_t mutex;
    pthread_cond_t readerCondition;
    pthread_cond_t writerCondition;
} PThreadReadWriteLock;

static PThreadReadWriteLock lock;

void readWriteLockInit() {
    lock.readers = 0;
    lock.writers = 0;
    lock.pendingWriters = 0;
    pthread_mutex_init(&lock.mutex, NULL);
    pthread_cond_init(&lock.readerCondition, NULL);
    pthread_cond_init(&lock.writerCondition, NULL);
}

/**
 * Get read lock
 * if no writer lock or pending writer lock,increase the number of readers and get  the lock
 * if there are writer lock or pending lock then release lock and wait for condition variable
 * @param lock ReadWriteLock implemented by PThread APIs
 */
void rLock() {
    pthread_mutex_lock(&lock.mutex);
    while (lock.writers > 0 || lock.pendingWriters > 0) {
        pthread_cond_wait(&lock.readerCondition, &lock.mutex);
    }
    lock.readers++;
    pthread_mutex_unlock(&lock.mutex);
}

/**
 * if there are readers or writers,wait for condition variable
 * if there is not any writers and readers get lock and decreasing the number of pending writers
 * @param lock
 */
void wLock() {
    pthread_mutex_lock(&lock.mutex);
    lock.pendingWriters++;
    while (lock.readers > 0 || lock.writers > 0) {
        pthread_cond_wait(&lock.writerCondition, &lock.mutex);
    }
    lock.pendingWriters--;
    lock.writers = 1;
    pthread_mutex_unlock(&lock.mutex);
}

/**
 * unlock read write lock
 * if there are any pending writers,we will signal the writer condition variable
 * else  we will signal all the readers condition variable
 * @param lock
 */
void unlockRWLock() {
    pthread_mutex_lock(&lock.mutex);
    if (lock.readers > 0) {
        lock.readers--;
    }
    else if (lock.writers>0) {
        lock.writers = 0;
    }
    if (lock.pendingWriters >0) {
        pthread_cond_signal(&lock.writerCondition);
    } else {
        pthread_cond_broadcast(&lock.readerCondition);
    }
    pthread_mutex_unlock(&lock.mutex);
}