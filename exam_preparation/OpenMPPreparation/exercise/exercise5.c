//
// Created by Chenx on 2025/10/15.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>
struct Node {
    int value;
    struct Node* left;
    struct Node* right;
    int size() { /* 返回子树节点数 */ }
};

int parallel_tree_sum(struct Node* node) {
    if (!node) return 0;
    if (node->size() == 1) return node->value;
    int left_sum = 0, right_sum = 0;
#pragma omp parallel
    {
#pragma omp single
        {
#pragma omp task shared(left_sum)
            {
                left_sum = parallel_tree_sum(node->left);
            }
#pragma omp task shared(right_sum)
            {
                right_sum = parallel_tree_sum(node->right);
            }
#pragma omp taskwait
        }
    }
    return left_sum + right_sum + node->value;
}