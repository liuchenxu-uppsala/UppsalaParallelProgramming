//
// Created by Chenx on 2025/10/22.
//
#include <stdio.h>
#include<omp.h>
#define N 4
#define M 5
#define L 5
int a[N][M][L];

int main() {
#pragma omp parallel
    {
#pragma omp for collapse(3) schedule(static)
        for (int n = 0; n < N; n++) {
            for (int m = 0; m < M; m++) {
                for (int l = 0; l < L; l++) {
                    a[n][m][l] = n*M*L+ m* L  + l;
                }
            }
        }
    }
}