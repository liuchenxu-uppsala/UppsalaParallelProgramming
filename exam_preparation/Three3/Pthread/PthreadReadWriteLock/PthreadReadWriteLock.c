//
// Created by Chenx on 2025/10/27.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    int readers;
    int writers;
    int pending_writers;
    pthread_mutex_t lock;
    pthread_cond_t reader_condition;
    pthread_cond_t writer_condition;
} P_ReadWriteLock;

P_ReadWriteLock p_read_write_lock;

void init_lock() {
    p_read_write_lock.readers = 0;
    p_read_write_lock.writers = 0;
    p_read_write_lock.pending_writers = 0;
    pthread_mutex_init(&p_read_write_lock.lock,NULL);
    pthread_cond_init(&p_read_write_lock.reader_condition,NULL);
    pthread_cond_init(&p_read_write_lock.writer_condition,NULL);
}

void r_lock() {
    pthread_mutex_lock(&p_read_write_lock.lock);
    while (p_read_write_lock.writers >0 || p_read_write_lock.pending_writers >0) {
        pthread_cond_wait(&p_read_write_lock.reader_condition,&p_read_write_lock.lock);
    }
    p_read_write_lock.readers++;
    pthread_mutex_unlock(&p_read_write_lock.lock);
}

void w_lock() {
    pthread_mutex_lock(&p_read_write_lock.lock);
    p_read_write_lock.pending_writers++;
    while (p_read_write_lock.writers > 0 || p_read_write_lock.readers >0) {
        pthread_cond_wait(&p_read_write_lock.writer_condition,&p_read_write_lock.lock);
    }
    p_read_write_lock.writers = 1;
    p_read_write_lock.pending_writers--;
    pthread_mutex_unlock(&p_read_write_lock.lock);
}

void un_lock() {
    pthread_mutex_lock(&p_read_write_lock.lock);
    if (p_read_write_lock.readers > 0) {
        p_read_write_lock.readers--;
    } else if (p_read_write_lock.writers > 0) {
        p_read_write_lock.writers = 0;
    }
    if (p_read_write_lock.pending_writers > 0) {
        pthread_cond_broadcast(&p_read_write_lock.writer_condition);
    } else {
        pthread_cond_broadcast(&p_read_write_lock.reader_condition);
    }
    pthread_mutex_unlock(&p_read_write_lock.lock);
}