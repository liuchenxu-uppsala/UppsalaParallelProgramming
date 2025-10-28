//
// Created by Chenx on 2025/10/28.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define CAPACITY 4
#define NUMS 9000000
typedef struct {
    int tail;
    int flags[CAPACITY];
    pthread_key_t key;
} a_lock_structure;

pthread_t threads[CAPACITY];
int tids[CAPACITY];
a_lock_structure a_lock;
int shared_value = 0;

void init_lock() {
    a_lock.tail = 0;
    for (int index = 0; index < CAPACITY; index++) {
        a_lock.flags[index] = 1;
    }
    a_lock.flags[0] = 0;
    pthread_key_create(&a_lock.key,NULL);
}

void set_slot(int index) {
    int* slot = pthread_getspecific(a_lock.key);
    if (slot == NULL) {
        slot = (int*)malloc(sizeof(int));
        pthread_setspecific(a_lock.key,slot);
    }
    *slot = index;
}

int get_slot() {
    return *(int*)pthread_getspecific(a_lock.key);
}

void __lock() {
    int index = __sync_fetch_and_add(&a_lock.tail,1);
    set_slot(index % CAPACITY);
    while (a_lock.flags[index % CAPACITY] == 1) ;
}

void __unlock() {
    int index = get_slot();
    a_lock.flags[index] = 1;
    a_lock.flags[(index + 1) % CAPACITY] = 0;
}

void* thread_function(void* arg) {
    int tid = *(int*) arg;
    for (int i = 0; i < NUMS; i++) {
        __lock();
        shared_value++;
        printf("thread:%d,shared_value:%d\n",tid,shared_value);
        __unlock();
    }
    return NULL;
}

int main() {
    init_lock();
    for (int i = 0; i < CAPACITY; i++) {
        tids[i] = i;
        pthread_create(&threads[i],NULL,thread_function,&tids[i]);
    }
    for (int i = 0; i < CAPACITY; i++) {
        pthread_join(threads[i],NULL);
    }
}