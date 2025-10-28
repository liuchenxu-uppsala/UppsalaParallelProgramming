//
// Created by Chenx on 2025/10/28.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define CAPACITY 4
#define NUMS 9000000
int shared_value = 0;
pthread_t threads[CAPACITY];
int tids[CAPACITY];
typedef struct {
    int lock;
} QNode;

typedef struct {
    QNode* tail;
    pthread_key_t my_key;
    pthread_key_t pre_key;
} CLH_lock;

CLH_lock clh_lock;

void init_lock() {
    QNode* init_node = malloc(sizeof(QNode));
    init_node->lock = 0;
    clh_lock.tail = init_node;
    pthread_key_create(&clh_lock.my_key,NULL);
    pthread_key_create(&clh_lock.pre_key,NULL);
}

QNode* get_node(pthread_key_t key) {
    QNode* node = (QNode*)pthread_getspecific(key);
    if (node == NULL) {
        if (key == clh_lock.my_key) {
            node = (QNode*)malloc(sizeof(QNode));
            pthread_setspecific(key,node);
        } else {
            return NULL;
        }
    }
    return node;
}

void set_node(pthread_key_t key, QNode* node) {
    pthread_setspecific(key,node);
}

void c_lock() {
    QNode* my_node = get_node(clh_lock.my_key);
    my_node->lock = 1;
    QNode* pre_node = __sync_lock_test_and_set(&clh_lock.tail,my_node);
    set_node(clh_lock.pre_key,pre_node);
    while (pre_node->lock == 1);
}

void c_unlock() {
    QNode* my_node = get_node(clh_lock.my_key);
    my_node->lock = 0;
    QNode* pre = get_node(clh_lock.pre_key);
    set_node(clh_lock.my_key,NULL);
}

void* thread_function(void* arg) {
    int tid = *(int*)arg;
    for (int index = 0; index < NUMS; index++) {
        c_lock();
        shared_value++;
        printf("thread:%d,shared_value:%d\n",tid,shared_value);
        c_unlock();
    }
    return NULL;
}

int main() {
    init_lock();
    for (int index = 0; index < CAPACITY; index++) {
        tids[index] = index;
        pthread_create(&threads[index],NULL,thread_function,&tids[index]);
    }
    for (int index = 0; index < CAPACITY; index++) {
        pthread_join(threads[index],NULL);
    }
}