//
// Created by Chenx on 2025/10/20.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define CAPACITY 4
#define MAX_NUMS 10000
int shared_value;
pthread_t threads[CAPACITY];
int tids[CAPACITY];
typedef struct {
    int tail;
    int flags[CAPACITY];
    pthread_key_t key;
} a_lock;
a_lock lock;

void init_lock() {
    lock.tail = 0;
    for (int i = 0; i < CAPACITY; i++) {
        lock.flags[i] = 0;
    }
    lock.flags[0] = 1;
    pthread_key_create(&lock.key,NULL);
}

void set_slot(int index) {
    int* slot = pthread_getspecific(lock.key);
    if (slot == NULL) {
        slot = (int*)malloc(sizeof(int));
        pthread_setspecific(lock.key,slot);
    }
    *slot = index;
}

int get_slot() {
    int* slot = pthread_getspecific(lock.key);
    // if (slot == NULL) {
    //     slot = (int*)malloc(sizeof(int));
    //     pthread_setspecific(lock.key,slot);
    //     *slot = 0;
    // }
    return *slot;
}

void _a_lock() {
    int index = __sync_fetch_and_add(&lock.tail,1);
    set_slot(index % CAPACITY);
    while (lock.flags[index % CAPACITY] == 0){}
}

void _unlock() {
    int index = get_slot();
    lock.flags[index % CAPACITY] = 0;
    lock.flags[(index + 1) % CAPACITY] = 1;
}

void* work(void* args) {
    int tid = *(int*)args;
    for (int i = 0; i < MAX_NUMS; i++) {
        _a_lock();
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