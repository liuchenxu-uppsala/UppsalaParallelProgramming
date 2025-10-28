//
// Created by Chenx on 2025/10/28.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define CAPACITY 100
#define MAX_NUMS 1000000
int shared_value = 0;

typedef struct q_node_t{
    int lock;
    struct q_node_t* next;
} mcs_node;

typedef struct {
    mcs_node* tail;
    pthread_key_t my_node_key;
} mcs_lock_structure_t;

mcs_lock_structure_t mcs_lock_structure;

pthread_t threads[CAPACITY];
int tids[CAPACITY];

void init_mcs() {
    mcs_lock_structure.tail = NULL;
    pthread_key_create(&mcs_lock_structure.my_node_key,NULL);
}

mcs_node* get_node() {
    mcs_node* node = pthread_getspecific(mcs_lock_structure.my_node_key);
    if (node == NULL) {
        node = (mcs_node*)malloc(sizeof(mcs_node));
        pthread_setspecific(mcs_lock_structure.my_node_key,node);
    }
    return node;
}

void set_node(mcs_node* node) {
    pthread_setspecific(mcs_lock_structure.my_node_key,node);
}

void mcs_lock() {
    mcs_node* node = get_node();
    node->lock = 1;
    node->next = NULL;
    mcs_node* pre = mcs_lock_structure.tail;
    while (__sync_val_compare_and_swap(&mcs_lock_structure.tail,pre,node) != pre) {
        pre = mcs_lock_structure.tail;
    }
    if (pre == NULL) {
        return;
    }
    pre->next = node;
    while (node->lock == 1);
}

void mcs_unlock() {
    mcs_node* node = get_node();
    if (node->next == NULL) {
        if (__sync_val_compare_and_swap(&mcs_lock_structure.tail,node,NULL) == node) {
            return;
        }
    }
    node->next->lock = 0;
}

void* thread_function(void* arg) {
    int tid = *(int*)arg;
    for (int idnex = 0; idnex < MAX_NUMS; ++idnex) {
        mcs_lock();
        shared_value++;
        printf("thread:%d,shared_value:%d\n",tid,shared_value);
        mcs_unlock();
    }
    return NULL;
}

int main() {
    init_mcs();
    for (int index = 0; index < CAPACITY; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,thread_function,&tids[index]);
    }
    for (int index = 0; index < CAPACITY; index++) {
        pthread_join(threads[index],NULL);
    }
}