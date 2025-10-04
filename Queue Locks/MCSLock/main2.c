#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h> // For memset

// --- 结构定义 ---

// MCS 队列节点
typedef struct q_node_t {
    // 锁状态：1 (true) = 等待中/已锁定, 0 (false) = 释放/可获取
    // 必须是 volatile，以便其他线程的写入对当前线程可见
    volatile unsigned char lock;
    volatile struct q_node_t* next;
} q_node;

// MCS 锁主体
typedef struct {
    // tail 必须是原子操作的目标。NULL 表示锁空闲。
    volatile q_node* tail;
} mcs_lock_t;

// --- 全局变量 ---
volatile mcs_lock_t global_mcs_lock;
pthread_key_t my_node_key;
unsigned long shared_value = 0;
#define TIMES 500
#define NUM_THREADS 10 // 使用 10 个线程进行测试

// --- 辅助函数：TLS 节点管理 ---

// 析构函数：线程退出时自动释放内存
void node_destructor(void *data) {
    if (data != NULL) free(data);
}

void init_mcs_key() {
    // 注册析构函数，防止线程退出时的内存泄漏
    pthread_key_create(&my_node_key, node_destructor);
}

void init_mcs_lock(mcs_lock_t* lock) {
    lock->tail = NULL;
}

q_node* get_node() {
    q_node* my_q_node = (q_node*)pthread_getspecific(my_node_key);

    if (my_q_node == NULL) {
        my_q_node = (q_node*)malloc(sizeof(q_node));
        // 确保分配的内存是干净的
        memset(my_q_node, 0, sizeof(q_node));
        pthread_setspecific(my_node_key, my_q_node);
    }

    // 每次使用前重置状态：准备入队，状态设置为等待 (1)
    my_q_node->lock = 1;
    my_q_node->next = NULL;

    return my_q_node;
}

// --- 锁核心函数 ---

void lock_mcs() {
    q_node* current_node = get_node();

    // 1. 原子交换：将当前节点加入队列尾部并获取前驱
    volatile q_node *pred_node = global_mcs_lock.tail;

    // CAS 循环模拟原子交换：确保原子地设置 tail 并获取旧值 (pred_node)
    while (__sync_val_compare_and_swap(&global_mcs_lock.tail, pred_node, current_node) != pred_node) {
        pred_node = global_mcs_lock.tail;
    }

    // 2. 检查前驱
    if (pred_node == NULL) {
        // 队列为空，成功获得锁
        return;
    }

    // 3. 队列不为空，链接前驱并自旋
    // 设置前驱的 next 指针，告知前驱它的后继是谁
    pred_node->next = current_node;

    // 自旋等待：在自己本地的 locked 字段上自旋
    while (current_node->lock == 1) {
        // 可以插入 CPU PAUSE 指令，减少能耗
    }
    // 线程成功获取锁
}

void un_lock_mcs() {
    q_node* current_node = (q_node*)pthread_getspecific(my_node_key);

    // 1. 检查当前线程是否是队列尾部（无后继）
    if (current_node->next == NULL) {
        // 尝试用 CAS 将 tail 从 current_node 设置为 NULL，释放锁
        if (__sync_val_compare_and_swap(&global_mcs_lock.tail, current_node, NULL) == current_node) {
            // CAS 成功，当前线程是最后一个，锁被释放。
            return;
        }

        // CAS 失败：有新线程在检查 next 和 CAS 之间加入了队列。
        // **必须等待新线程设置 next 指针** (竞争窗口处理)
        while (current_node->next == NULL) {
            // 自旋等待链接完成！
        }
    }

    // 2. 唤醒后继线程
    // 此时 current_node->next 保证不是 NULL。将后继节点的 lock 设置为 0，传递令牌。
    current_node->next->lock = 0;
}

// --- Demo 逻辑 ---

void* thread_func(void* arg) {
    int tid = *(int*)arg;

    lock_mcs();

    // 临界区
    // printf("Thread id:%d enter mutual area\n",tid); // 打印过多会干扰性能测试
    for (int i = 0; i < TIMES; i++) {
        shared_value++;
    }

    un_lock_mcs();
    return NULL;
}

int main(void) {
    printf("MCS Lock Test with %d threads and %d iterations each\n", NUM_THREADS, TIMES);

    init_mcs_key();
    init_mcs_lock(&global_mcs_lock);

    int tids[NUM_THREADS];
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        tids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_func, &tids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // --- 结果检查 ---
    long expected = (long)NUM_THREADS * TIMES;
    printf("\nExpected value: %ld\n", expected);
    printf("Actual shared_value: %lu\n", shared_value);

    if (shared_value == expected) {
        printf("Result: SUCCESS! Lock is working correctly.\n");
    } else {
        printf("Result: FAILURE! Shared value is incorrect.\n");
    }

    // 清理 Key
    pthread_key_delete(my_node_key);

    return 0;
}