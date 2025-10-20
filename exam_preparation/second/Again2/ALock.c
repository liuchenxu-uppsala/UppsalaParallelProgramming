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
#define MAX_TIMES 100
int shared_value = 0;

typedef struct {
    volatile int tail;
    volatile int flag[CAPACITY];
    pthread_key_t key;
} ALock;
ALock alock;

void init_lock() {
    alock.tail = 0;
    for (int i = 0; i < CAPACITY; i++) {
        alock.flag[i] = 0;
    }
    alock.flag[0] = 1;
    pthread_key_create(&alock.key,NULL);
}

int get_slot() {
    int* slot = pthread_getspecific(alock.key);
    if (slot == NULL) {
        slot = (int*) malloc(sizeof(int));
        *slot = 0;
        pthread_setspecific(alock.key,slot);
    }
    return *slot;
}

void set_slot(int slot_index) {
    int* slot = pthread_getspecific(alock.key);
    if (slot == NULL) {
        slot = (int*) malloc(sizeof(int));
        pthread_setspecific(alock.key,slot);
    }
    *slot = slot_index;
}

void lock() {
    int my_index = __sync_fetch_and_add(&alock.tail,1) % CAPACITY;
    set_slot(my_index);
    while (alock.flag[my_index] == 0 ) {}
}

void unlock() {
    int slot_index = get_slot();
    alock.flag[slot_index] = 0;
    alock.flag[(slot_index + 1) % CAPACITY] = 1;
}

pthread_t threads[CAPACITY];
int tids[CAPACITY];

void* work(void* arg) {
    int tid = *(int*) arg;
    for (int i = 0; i < MAX_TIMES; i++) {
        lock();
        shared_value++;
        printf("thread:%d,shared_value:%d\n",tid,shared_value);
        unlock();
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
    return 0;
}