//
// Created by Chenx on 2025/10/29.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define CAPACITY 4
#define NUMS 9000000
typedef struct {
    pthread_key_t key_my_slot;
    int flags[CAPACITY];
    int tail;
} a_lock_t;
pthread_t treads[CAPACITY];
int tids[CAPACITY];
a_lock_t a_lock;
int shared_value = 0;

void init() {
    a_lock.tail = 0;
    pthread_key_create(&a_lock.key_my_slot,NULL);
    for (int index = 0; index < CAPACITY; index++) {
        a_lock.flags[index] = 1;
    }
    a_lock.flags[0] = 0;
}

void set_slot(int index) {
    int* my_slot = (int*)pthread_getspecific(a_lock.key_my_slot);
    if (my_slot == NULL) {
        my_slot = (int*)malloc(sizeof(int));
        pthread_setspecific(a_lock.key_my_slot,my_slot);
    }
    *my_slot = index;
}

int get_slot() {
    return *(int*)pthread_getspecific(a_lock.key_my_slot);
}

void lock() {
    int my_slot = __sync_fetch_and_add(&a_lock.tail,1);
    set_slot(my_slot % CAPACITY);
    while (a_lock.flags[my_slot % CAPACITY] == 1);
}

void un_lock() {
    int my_slot = get_slot();
    a_lock.flags[my_slot] = 1;
    a_lock.flags[(my_slot + 1) % CAPACITY] = 0;
}

void* thread_function(void* arg) {
    int tid = *(int*) arg;
    for (int i = 0; i < NUMS; i++) {
        lock();
        shared_value++;
        printf("thread:%d,shared_value:%d\n",tid,shared_value);
        un_lock();
    }
}

int main() {
    init();
    for (int index = 0; index < CAPACITY; index++) {
        tids[index] = index;
        pthread_create(&treads[index],NULL,thread_function,&tids[index]);
    }
    for (int index = 0; index < CAPACITY; index++) {
        pthread_join(treads[index],NULL);
    }
}