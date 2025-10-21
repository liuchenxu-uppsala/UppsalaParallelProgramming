//
// Created by Chenx on 2025/10/21.
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

typedef struct q_node_t {
    volatile unsigned char lock;
    volatile struct q_node_t* next;
} q_node;

typedef struct {
    q_node* tail;
} ncs_lock_t;

pthread_key_t my_key;
ncs_lock_t ncs_lock;

pthread_t threads[CAPACITY];
int tids[CAPACITY];

q_node* get_node(pthread_key_t key) {
    q_node* node = pthread_getspecific(key);
    if (node == NULL) {
        node = (q_node*) malloc(sizeof(q_node));
        node->next = NULL;
        pthread_setspecific(key,node);
    }
    return node;
}

void set_node(pthread_key_t key, q_node* node) {
    pthread_setspecific(key,node);
}

void lock() {
    q_node* my_node = get_node(my_key);
    my_node->next = NULL;
    my_node->lock = 1;
    q_node* pre_tail = ncs_lock.tail;
    while (__sync_val_compare_and_swap(&ncs_lock.tail,pre_tail,my_node) != pre_tail) {
        pre_tail = ncs_lock.tail;
    }
    if (pre_tail == NULL) {
        return;
    }
    pre_tail->next = my_node;
    while (my_node->lock == 1){}
}

void unlock() {
    q_node *my_node = get_node(my_key);
    if (my_node->next == NULL) {
        if (__sync_val_compare_and_swap(&ncs_lock.tail, my_node,NULL) == my_node) {
            return;
        }
    }
    my_node->next->lock = 0;
}

void init_lock() {
    ncs_lock.tail = NULL;
    pthread_key_create(&my_key,NULL);
}

void* work(void* arg) {
    int tid = *(int*) arg;
    for (int index = 0; index < MAX_NUMS; index++) {
        lock();
        shared_value++;
        printf("Thread:%d,shared_value:%d\n",tid,shared_value);
        unlock();
    }
    return NULL;
}

int main() {
    init_lock();
    for (int index = 0; index < CAPACITY; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,work,&tids[index]);
    }
    for (int index = 0; index < CAPACITY; index++) {
        pthread_join(threads[index],NULL);
    }
    return 0;
}

