//
// Created by Chenx on 2025/10/14.
//
#include "pthread.h"
typedef struct {
    int readers;
    int writers;
    int pending_writers;
    pthread_mutex_t lock;
    pthread_cond_t reader_cond_t;
    pthread_cond_t writer_cond_t;
} Reader_Writer_Lock;

Reader_Writer_Lock reader_writer_lock;

void init_reader_writer_lock() {
    reader_writer_lock.readers = 0;
    reader_writer_lock.writers = 0;
    reader_writer_lock.pending_writers = 0;
    pthread_mutex_init(&reader_writer_lock.lock,NULL);
    pthread_cond_init(&reader_writer_lock.reader_cond_t,NULL);
    pthread_cond_init(&reader_writer_lock.writer_cond_t,NULL);
}

void read_lock() {
    pthread_mutex_lock(&reader_writer_lock.lock);
    while (reader_writer_lock.writers > 0 || reader_writer_lock.pending_writers >0) {
        pthread_cond_wait(&reader_writer_lock.reader_cond_t,&reader_writer_lock.lock);
    }
    reader_writer_lock.readers++;
    pthread_mutex_unlock(&reader_writer_lock.lock);
}

void write_lock() {
    pthread_mutex_lock(&reader_writer_lock.lock);
    reader_writer_lock.pending_writers++;
    while (reader_writer_lock.writers > 0 || reader_writer_lock.readers > 0) {
        pthread_cond_wait(&reader_writer_lock.writer_cond_t,&reader_writer_lock.lock);
    }
    reader_writer_lock.pending_writers--;
    reader_writer_lock.writers = 1;
    pthread_mutex_unlock(&reader_writer_lock.lock);
}

void read_write_unlock() {
    pthread_mutex_lock(&reader_writer_lock.lock);
    if (reader_writer_lock.readers > 0) {
        reader_writer_lock.readers--;
    } else if (reader_writer_lock.writers > 0) {
        reader_writer_lock.writers = 0;
    }
    if (reader_writer_lock.pending_writers > 0) {
        pthread_cond_broadcast(&reader_writer_lock.writer_cond_t);
    } else {
        pthread_cond_broadcast(&reader_writer_lock.reader_cond_t);
    }
    pthread_mutex_unlock(&reader_writer_lock.lock);
}
