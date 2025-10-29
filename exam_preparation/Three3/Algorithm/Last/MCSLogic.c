//
// Created by Chenx on 2025/10/29.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define CAPACITY 100
#define NUMS 10000
long shared_value = 0;
typedef struct QNode {
    struct QNode* next;
    int lock_status;
} QNode_t;

typedef struct {
    struct QNode* tail;
    pthread_key_t key_my_node;
} mcs_structure;

pthread_t threads[CAPACITY];
int tids[CAPACITY];
mcs_structure mcs_t;

void init() {
    mcs_t.tail = NULL;
    pthread_key_create(&mcs_t.key_my_node,NULL);
}

QNode_t* get_node() {
    QNode_t* node = pthread_getspecific(mcs_t.key_my_node);
    if (node == NULL) {
        node = (QNode_t*)malloc(sizeof(QNode_t));
        node->next = NULL;
        node->lock_status = 1;
        pthread_setspecific(mcs_t.key_my_node,node);
    }
    return node;
}

void set_node(pthread_key_t key,QNode_t* my_node) {
    pthread_setspecific(key,my_node);
}

void lock() {
    QNode_t* my_node = get_node();
    my_node->next = NULL;
    my_node->lock_status = 1;
    QNode_t* pre = mcs_t.tail;
    while (__sync_val_compare_and_swap(&mcs_t.tail,pre,my_node) != pre) {
        pre = mcs_t.tail;
    }
    if (pre == NULL) {
        return;
    }
    pre->next = my_node;
    while (my_node->lock_status == 1);
}

void unlock() {
    QNode_t* my_node = get_node();
    if (my_node->next == NULL) {
        if (__sync_val_compare_and_swap(&mcs_t.tail,my_node,NULL) == my_node) {
            return;
        }
    }
    my_node->next->lock_status = 0;
}

void* thread_function(void* arg) {
    int tid = *(int*) arg;
    for (int index = 0; index < NUMS; index++) {
        lock();
        shared_value++;
        printf("thread:%d,shared_value:%ld\n",tid,shared_value);
        unlock();
    }
}

int main() {
    init();
    for (int index = 0; index < CAPACITY; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,thread_function,&tids[index]);
    }
    for (int index = 0; index < CAPACITY; index++) {
        pthread_join(threads[index],NULL);
    }
}

