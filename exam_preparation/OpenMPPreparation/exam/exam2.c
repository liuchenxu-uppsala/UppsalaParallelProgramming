//
// Created by Chenx on 2025/10/15.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
int getDepth();
int main() {
    #pragma omp parallel num_threads(16)
    {
    #pragma omp single
        {
            getDepth();
        }
    }
}

int getDepth(){
    int leftH,rightH;
    if (this.left != null) {
#pragma omp task shared(leftH) if(this.left.size() > 10)
        {
            leftH = getDepth(this.left);
        }
    } else {
        leftH = 0;
    }

    if (this.right != null) {
#pragma omp task shared(rightH) if(this.left.size() > 10)
        {
            rightH = getDepth(this.right);
        }
    } else {
        rightH = 0;
    }

#pragma omp taskwait
    return 1 + max(leftH,rightH);
}