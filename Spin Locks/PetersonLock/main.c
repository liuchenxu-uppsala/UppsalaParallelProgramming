#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
volatile unsigned int victim = 0;
volatile bool flags[2] = {false,false};
volatile int shared_value = 0;

pthread_t threads[2];
int thread_ids[2] = {0,1};

void lock(unsigned int tid) {
    int id = tid;
    int j = 1 -id;
    flags[id] = true;
    victim = id;
    while (flags[j] && victim == id) {
    }
    printf("Thread %d has locked the mutex\n", id);
}

void unlock(unsigned int tid) {
    flags[tid] = false;
}

void* function(void * arg) {
    int tid = *(int*)arg;
    lock(tid);
    for (int i = 0; i < 5; i++) {
        int temp = shared_value;
        temp =  temp + 1;
        shared_value = temp;
    }
    printf("Thread %d has incremented the shared value to %d\n", tid, shared_value);
    unlock(tid);
    return NULL;
}

int main(void) {
    printf("Hello, World!\n");
    pthread_create(&threads[0], NULL, function, &thread_ids[0]);
    pthread_create(&threads[1], NULL, function, &thread_ids[1]);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    return 0;
}