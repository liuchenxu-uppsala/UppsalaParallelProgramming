//
// Created by Chenx on 2025/10/21.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define CAPACITY 4
#define MAX_NUMS 100
pthread_t threads[CAPACITY];
int shared_value = 0;
int tids[CAPACITY];

typedef struct {
    int tail;
    int flags[CAPACITY];
    pthread_key_t key;
} lock;

lock a_lock;

void init_lock() {
    a_lock.tail = 0;
    for (int i = 0; i < CAPACITY; i++) {
        a_lock.flags[i] = 0;
    }
    a_lock.flags[0] = 1;
    pthread_key_create(&a_lock.key,NULL);
}

void set_slot(int index) {
    int* slot = pthread_getspecific(a_lock.key);
    if (slot == NULL) {
        slot = (int*) malloc(sizeof(int));
        pthread_setspecific(a_lock.key,slot);
    }
    *slot = index;
}

int get_slot() {
    int* slot = pthread_getspecific(a_lock.key);
    return *slot;
}

void _lock() {
    int my_index = __sync_fetch_and_add(&a_lock.tail,1);
    set_slot(my_index % CAPACITY);
    while (a_lock.flags[my_index % CAPACITY] == 0);
}

void _unlock() {
    int slot = *(int*)pthread_getspecific(a_lock.key);
    a_lock.flags[slot] = 0;
    a_lock.flags[(slot + 1) % CAPACITY] = 1;
}

void* work(void* arg) {
    int tid = *(int*) arg;
    for (int i = 0; i < MAX_NUMS; i++) {
        _lock();
        shared_value++;
        printf("Thread:%d,shared_value:%d\n",tid,shared_value);
        _unlock();
    }
}

int main() {
    init_lock();
    for (int i = 0; i < CAPACITY; i++) {
        tids[i] = i;
        pthread_create(&threads[i],NULL,work,&tids[i]);
    }
    for (int i = 0; i < CAPACITY; i++) {
        pthread_join(threads[i],NULL);
    }
    printf("shared_value:%d\n",shared_value);
}