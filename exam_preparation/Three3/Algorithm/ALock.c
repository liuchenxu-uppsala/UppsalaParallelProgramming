//
// Created by Chenx on 2025/10/28.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 4
#define MAX_NUM 1000000

pthread_t threads[CAPACITY];
int tids[CAPACITY];
int shared_value = 0;

typedef struct {
    int tail;
    pthread_key_t key;
    int flags[CAPACITY];
} a_lock_struct;

a_lock_struct a_lock;

void init_a_lock() {
    a_lock.tail = 0;
    for (int index = 0; index < CAPACITY; ++index) {
        a_lock.flags[index] = 1;
    }
    a_lock.flags[0] = 0;
    pthread_key_create(&a_lock.key,NULL);
}

void set_slot(int index) {
    int* slot = (int*)pthread_getspecific(a_lock.key);
    if (slot == NULL) {
        slot = (int*) malloc(sizeof(int));
        pthread_setspecific(a_lock.key,slot);
    }
    *slot = index;
}

int get_slot() {
    int* value = (int*)pthread_getspecific(a_lock.key);
    return *value;
}

void _lock() {
    int index = __sync_fetch_and_add(&a_lock.tail,1);
    set_slot(index % CAPACITY);
    while (a_lock.flags[index % CAPACITY] == 1);
}

void _unlock(){
    int slot = get_slot();
    a_lock.flags[slot] = 1;
    a_lock.flags[(slot + 1) % CAPACITY] = 0;
}

void* thread_function(void* arg) {
    int tid = *(int*)arg;
    for (int i = 0; i < MAX_NUM; i++) {
        _lock();
        shared_value++;
        printf("thread:%d,shared_value:%d\n",tid,shared_value);
        _unlock();
    }
    return NULL;
}

int main() {
    init_a_lock();
    for (int index = 0; index < CAPACITY; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,thread_function,&tids[index]);
    }
    for (int index = 0; index < CAPACITY; index++) {
        pthread_join(threads[index],NULL);
    }
}