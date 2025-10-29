//
// Created by Chenx on 2025/10/29.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define CAPACITY 100
#define NUMS 10000

pthread_t threads[CAPACITY];
long shared_value = 0;
int tids[CAPACITY];

typedef struct {
    int flag;
} QNode;

typedef struct {
    QNode* tail;
    pthread_key_t key_my_node;
    pthread_key_t key_pre_node;
} clh_structure;

clh_structure clh;

void init() {
    clh.tail = (QNode*) malloc(sizeof(QNode));
    clh.tail->flag = 0;
    pthread_key_create(&clh.key_my_node,NULL);
    pthread_key_create(&clh.key_pre_node,NULL);
}

void set_slot(pthread_key_t key,QNode* qnode) {
    pthread_setspecific(key,qnode);
}

QNode* get_slot(pthread_key_t key) {
    QNode* temp = pthread_getspecific(key);
    if (temp == NULL) {
        if (key == clh.key_my_node) {
            temp = (QNode*)malloc(sizeof(QNode));
            pthread_setspecific(key,temp);
        } else {
            return NULL;
        }
    }
    return temp;
}

void lock(){
    QNode* my_node = get_slot(clh.key_my_node);
    my_node->flag = 1;
    QNode* pre = __sync_lock_test_and_set(&clh.tail,my_node);
    set_slot(clh.key_pre_node,pre);
    while (pre->flag == 1) ;
}

void unlock() {
    QNode* my_node = get_slot(clh.key_my_node);
    my_node->flag = 0;
    set_slot(clh.key_my_node,NULL);
}

void* thread_function(void* arg) {
    int tid = *(int*) arg;
    for (int index = 0; index < NUMS; ++index) {
        lock();
        shared_value++;
        printf("thread:%d,shared_value:%d\n",tid,shared_value);
        unlock();
    }
    return NULL;
}

int main() {
    init();
    for (int i = 0; i < CAPACITY; i++) {
        tids[i] = i;
        pthread_create(&threads[i],NULL,thread_function,&tids[i]);
    }
    for (int i = 0; i < CAPACITY; i++) {
        pthread_join(threads[i],NULL);
    }
}