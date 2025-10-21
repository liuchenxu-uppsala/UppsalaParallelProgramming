//
// Created by Chenx on 2025/10/21.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define CAPACITY 10
#define MAX_NUMS 100
int shared_value = 0;

pthread_t threads[CAPACITY];
int tids[CAPACITY];

typedef struct {
    int lock;
} QNode;

typedef struct {
    QNode* tail;
    pthread_key_t my_node;
    pthread_key_t pre_node;
} clh_lock;

clh_lock c_lock;

void init_lock() {
    QNode* init_node = (QNode*) malloc(sizeof(QNode));
    init_node->lock = 0;
    pthread_key_create(&c_lock.my_node,NULL);
    pthread_key_create(&c_lock.pre_node,NULL);
    c_lock.tail = init_node;
}

QNode* get_node(pthread_key_t key) {
    QNode* node = pthread_getspecific(key);
    if (node == NULL) {
        if (key == c_lock.my_node) {
            node = (QNode*) malloc(sizeof(QNode));
            node->lock = 0;
            pthread_setspecific(key,node);
        } else {
            return NULL;
        }
    }
    return node;
}

void set_node(pthread_key_t key, QNode* node) {
    pthread_setspecific(key, node);
}

void lock() {
    QNode* my_node = get_node(c_lock.my_node);
    my_node->lock = 1;
    QNode* pre = __sync_lock_test_and_set(&c_lock.tail,my_node);
    while (pre->lock == 1) {}
}

void unlock() {
    QNode* my_node = get_node(c_lock.my_node);
    my_node->lock = 0;
    // QNode *pred = get_node(c_lock.pre_node);
    // set_node(c_lock.my_node, NULL);
}

void* work(void* arg) {
    int tid = *(int*) arg;
    for (int index = 0; index < MAX_NUMS; index++) {
        lock();
        shared_value++;
        printf("Thread:%d,shared_value:%d\n",tid,shared_value);
        unlock();
    }
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
    printf("shared_value:%d\n",shared_value);
}

