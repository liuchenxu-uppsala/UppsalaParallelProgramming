//
// Created by Chenx on 2025/10/22.
//
#include "pthread.h"

typedef struct {
    int readers;
    int writers;
    int pending_writers;
    pthread_mutex_t p_lock;
    pthread_cond_t reader_condition;
    pthread_cond_t writer_condition;
} read_write_t;

read_write_t read_write;

void init() {
    read_write.readers = 0;
    read_write.writers = 0;
    read_write.pending_writers = 0;
    pthread_mutex_init(&read_write.p_lock,NULL);
    pthread_cond_init(&read_write.reader_condition,NULL);
    pthread_cond_init(&read_write.writer_condition,NULL);
}

void read_lock() {
    pthread_mutex_lock(&read_write.p_lock);
    while (read_write.writers > 0 || read_write.pending_writers > 0) {
        pthread_cond_wait(&read_write.reader_condition,&read_write.p_lock);
    }
    read_write.readers++;
    pthread_mutex_unlock(&read_write.p_lock);
}

void write_lock() {
    pthread_mutex_lock(&read_write.p_lock);
    read_write.pending_writers++;
    while (read_write.writers > 0 || read_write.readers > 0) {
        pthread_cond_wait(&read_write.writer_condition,&read_write.p_lock);
    }
    read_write.pending_writers--;
    read_write.writers = 1;
    pthread_mutex_unlock(&read_write.p_lock);
}

void read_write_unlock() {
    pthread_mutex_lock(&read_write.p_lock);
    if (read_write.readers > 0) {
        read_write.readers--;
    } else if (read_write.writers > 0) {
        read_write.writers = 0;
    }
    if (read_write.pending_writers > 0) {
        pthread_cond_broadcast(&read_write.writer_condition);
    } else {
        pthread_cond_broadcast(&read_write.reader_condition);
    }
    pthread_mutex_unlock(&read_write.p_lock);
}