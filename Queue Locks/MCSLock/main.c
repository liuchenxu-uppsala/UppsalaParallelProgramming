#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef struct q_node_t {
    volatile unsigned char lock;
    volatile struct q_node_t* next;
} q_node;

typedef struct {
    volatile q_node* tail;
} mcs_lock;

volatile mcs_lock lock;
pthread_key_t my_node;
unsigned long shared_value = 0;
#define TIMES 500

pthread_t threads[10];
int tids[10];

void init_mcs_lock(mcs_lock* lock) {
    lock->tail = NULL;
}

q_node* get_node() {
    q_node* my_q_node = (q_node*)pthread_getspecific(my_node);
    if (my_q_node == NULL) {
        my_q_node = (q_node*)malloc(sizeof(q_node));
        pthread_setspecific(my_node, my_q_node);
    }
    return my_q_node;
}

void lock_mcs() {
    q_node* current_node = get_node();
    current_node->lock = 1;
    current_node->next = NULL;
    volatile q_node *pred_node = lock.tail;
    while (__sync_val_compare_and_swap(&lock.tail, pred_node, current_node) != pred_node) {
        pred_node = lock.tail;
    }
    if (pred_node == NULL) {
        return;
    }
    pred_node->next = current_node;
    while (current_node->lock == 1) {
    }
}

void un_lock_mcs() {
    q_node* current_node = (q_node*)pthread_getspecific(my_node);
    if (current_node->next == NULL) {
        if (__sync_val_compare_and_swap(&lock.tail, current_node, NULL) == current_node) {
            return;
        }
    }
    current_node->next->lock = 0;
}

void* thread_func(void* arg) {
    lock_mcs();
    int tid = *(int*)arg;
    printf("Thread id:%d enter mutual area\n",tid);
    for (int i = 0; i < TIMES; i++) {
        shared_value++;
    }
    printf("Thread id:%d left mutual area,shared value:%ld\n",tid,shared_value);
    un_lock_mcs();
    return NULL;
}

int main(void) {
    printf("Hello, World!\n");
    pthread_key_create(&my_node, NULL);
    init_mcs_lock(&lock);
    for (int i = 0; i < 100; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &tids[i]);
    }
    for (int i = 0; i < 100; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}