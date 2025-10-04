#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
typedef struct q_node {
    _Atomic struct q_node* pred;
} q_node;
typedef struct {
    _Atomic struct q_node* tail;
    pthread_t current_node;
} to_lock;

static q_node AVAILABLE_NODE = { .pred = NULL };
#define AVAILABLE (&AVAILABLE_NODE)
to_lock global_lock;
int shared_resource = 0;
int THREAD_COUNT = 5;

void init_lock(to_lock* lock) {
    atomic_store(&lock->tail,AVAILABLE);
    pthread_key_create(&lock->current_node,NULL);
}

q_node* get_node(to_lock* lock) {
    q_node* node = pthread_getspecific(lock->current_node);
    if (node == NULL) {
        node = (q_node*) malloc(sizeof(q_node));
        node->pred = NULL;
    }
    pthread_setspecific(lock->current_node,node);
    return node;
}

long get_current_ms() {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return spec.tv_sec * 1000 + spec.tv_nsec / 1000000;
}

int lock(to_lock* lock, long patience_ms) {
    q_node* current = get_node(lock);
    atomic_store(&current->pred, NULL);
    q_node* current_pred = (q_node*)atomic_exchange(&lock->tail,current);
    if (current_pred == AVAILABLE) {
        return 1;
    }
    long start_time_ms = get_current_ms();
    q_node *pred_to_wait_on = current_pred;
    while (get_current_ms() - start_time_ms < patience_ms) {
        q_node *predPred = atomic_load(&pred_to_wait_on->pred);
        if (predPred == AVAILABLE) {
            return 1;
        } else if (predPred != NULL) {
            pred_to_wait_on = predPred;
        }
        usleep(100);
    }
    q_node *expected = current;
    q_node *desired = pred_to_wait_on;
    if (!atomic_compare_exchange_strong(&lock->tail, &expected, desired)) {
        atomic_store(&current->pred, AVAILABLE);
    }
    return 0;
}

void un_lock(to_lock* lock) {
    q_node *qnode = get_node(lock);
    if (qnode == NULL) {
        return;
    }
    q_node *expected = qnode;
    q_node *desired = AVAILABLE;
    if (!atomic_compare_exchange_strong(&lock->tail, &expected, desired)) {
        atomic_store(&qnode->pred, AVAILABLE);
    }
}

void *thread_func(void *arg) {
    long thread_id = (long)arg;
    int acquired = lock(&global_lock, 500 * (thread_id % 3 + 1));
    if (acquired) {
        printf("[%lu] CRITICAL: Lock acquired. Accessing resource.\n", thread_id);
        usleep(100000 * (thread_id % 2 + 1));
        shared_resource++;
        un_lock(&global_lock);
        printf("[%lu] RELEASE: Lock released.\n", thread_id);
    } else {
        printf("[%lu] ABANDONED: Could not acquire lock within timeout.\n", thread_id);
    }
    return NULL;
}

int main(void) {
    printf("Hello, World!\n");
    pthread_t threads[THREAD_COUNT];
    init_lock(&global_lock);
    printf("--- TOLock Demo Started ---\n");
    for (long i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void *)(i + 1));
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("--- TOLock Demo Finished ---\n");
    printf("Final shared resource value: %d\n", shared_resource);
    pthread_key_delete(global_lock.current_node);
    return 0;
    return 0;
}