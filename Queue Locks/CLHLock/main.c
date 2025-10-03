#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 4
unsigned long shared_value = 0;
pthread_t threads[MAX_THREADS];
int thread_ids[MAX_THREADS];

typedef struct {
    volatile int lock;
} QNode;

typedef struct {
    pthread_key_t my_node;
    pthread_key_t pre_node;
    QNode *tail;
} CLHLock;

CLHLock clh_lock;

void init_clh() {
    QNode* init_node = (QNode*)malloc(sizeof(QNode));
    init_node->lock = 0;
    clh_lock.tail = init_node;
    pthread_key_create(&clh_lock.my_node, NULL);
    pthread_key_create(&clh_lock.pre_node, NULL);
}

QNode* get_lock(pthread_key_t key) {
    QNode* my_lock = pthread_getspecific(key);
    if (my_lock == NULL) {
        if (key == clh_lock.my_node) {
            my_lock = (QNode*) malloc(sizeof(QNode));
            my_lock->lock = 0;
        } else {
            my_lock = NULL;
        }
        pthread_setspecific(key, my_lock);
    }
    return my_lock;
}

void set_node(pthread_key_t key, QNode* node) {
    pthread_setspecific(key, node);
}

void lock() {
    QNode* my_node = get_lock(clh_lock.my_node);
    my_node->lock = 1;
    QNode* pre_node = __sync_lock_test_and_set(&clh_lock.tail, my_node);
    set_node(clh_lock.pre_node,pre_node);
    while (pre_node->lock == 1) {
    }
}

void un_lock() {
    QNode* my_node = get_lock(clh_lock.my_node);
    my_node->lock = 0;
}

void*  thread_func(void *arg) {
    int tid =  *(int *)arg;
    lock();
    printf("Thread %d has acquired the lock\n", tid);
    for (int  i = 0; i < 500; i++) {
        shared_value++;
    }
    printf("Thread %d has released the lock,shared_value:%ld\n", tid, shared_value);
    un_lock();
    return NULL;
}

int main(void) {
    printf("Hello, World!\n");
    init_clh();
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
    }
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}