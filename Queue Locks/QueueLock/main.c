#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define CAPACITY 1000
#define MAXTHREADS 1000

typedef struct {
    volatile int tail;
    volatile int flags[CAPACITY];
    pthread_key_t slot_index;
} aLock;

aLock lock;
pthread_t threads[MAXTHREADS];
int threadIds[MAXTHREADS];
int nums_per_loop = 10;
long shared_value = 0;

void init_aLock() {
    lock.tail = 0;
    for (int i = 0; i < CAPACITY; i++) {
        lock.flags[i] = 0;
    }
    lock.flags[0] = 1;
    if (pthread_key_create(&lock.slot_index, NULL) !=0) {
        perror("pthread_key_create failed");
        exit(EXIT_FAILURE);
    }
}

int getSlot() {
    int* slot = (int*)pthread_getspecific(lock.slot_index);
    if (slot == NULL) {
        slot = (int*) malloc(sizeof(int));
        *slot = 0;
        pthread_setspecific(lock.slot_index, slot);
    }
    return *slot;
}

void setSlot(int slot) {
    int* slot_ptr = (int*)pthread_getspecific(lock.slot_index);
    if (slot_ptr == NULL) {
        slot_ptr = (int*) malloc(sizeof(int));
        *slot_ptr = 0;
        pthread_setspecific(lock.slot_index, slot_ptr);
    }
    *slot_ptr = slot;
}

void lock_a() {
    int old_slot = __sync_fetch_and_add(&lock.tail, 1);
    setSlot(old_slot);
    while (lock.flags[old_slot] == 0) {
    }
}

void unlock_a() {
    int slot = getSlot();
    lock.flags[slot] = 0;
    lock.flags[slot + 1] = 1;
}

void* function(void* arg) {
    int id = *(int*)arg;
    lock_a();
    printf("Thread %d has entered critical section\n", id);
    for (int i = 0; i < nums_per_loop; i++) {
        shared_value++;
    }
    printf("Thread %d has left critical section,shared_value:%ld\n", id, shared_value);
    unlock_a();
    return NULL;
}

int main(void) {
    printf("Hello, World!\n");
    init_aLock();
    for (int i = 0; i < MAXTHREADS; i++) {
        threadIds[i] = i;
        pthread_create(&threads[i], NULL, function, &threadIds[i]);
    }
    for (int i = 0; i < MAXTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}