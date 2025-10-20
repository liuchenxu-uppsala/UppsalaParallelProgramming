#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

/**
* 因此 volatile 不能保证：
** 内存顺序（Memory Ordering）
** CPU 可能会乱序执行读写操作。
** 多核可见性（Visibility）
** 写入的数据可能还在一个核的缓存里，另一个核看不到。
** 原子性（Atomicity）
** 对于多字节数据（如 int64_t），写入可能被拆成多个机器指令。
 */
volatile int flags[2] = {false,false};
volatile int victim;
pthread_t threads[2];
int tids[2] = {0,1};
volatile int shared_value = 0;

void* work(void* arg);

int main(void) {
    printf("Hello, World!\n");
    pthread_create(&threads[0],NULL,work,&tids[0]);
    pthread_create(&threads[1],NULL,work,&tids[1]);
    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    printf("final shared_value:%d\n",shared_value);
    return 0;
}

void lock(int tid) {
    flags[tid] = true;
    int j = 1 - tid;
    victim = j;
    while (flags[j] && victim == tid) {
    }
}

void unlock(int tid) {
    flags[tid] = false;
}

void* work(void* arg) {
    int tid = *(int*)arg;
    for (int i = 0; i < 100; i++) {
        lock(tid);
        shared_value = shared_value + 1;
        printf("Thread:%d,shared_value:%d\n",tid,shared_value);
        unlock(tid);
    }
}