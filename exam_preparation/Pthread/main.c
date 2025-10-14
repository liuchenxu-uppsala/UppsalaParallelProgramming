#include <stdio.h>
#include <stdlib.h>

#include "pthread.h"
int THREADS_NUM= 0;
long samplePoints;
int mum_threads;
long points_per_thread;
long hits;
#define MAX_THREADS 512
long hits_perthread[MAX_THREADS];
pthread_t threads[MAX_THREADS];
void* compute_pi(void* arg);
int main(void) {
    printf("How many points do you want to use?\n");
    scanf("%ld", &samplePoints);
    printf("How many threads do you want to use?\n");
    scanf("%d", &mum_threads);
    srand((unsigned int)time(NULL));
    points_per_thread = samplePoints / mum_threads;
    for (int index = 0; index< mum_threads; index++) {
        pthread_create(&threads[index],NULL,compute_pi,&hits_perthread[index]);
    }
    for (int index = 0; index< mum_threads; index++) {
        pthread_join(threads[index],NULL);
    }
    for (int index = 0; index< mum_threads; index++) {
        hits = hits + hits_perthread[index];
    }
    printf("Pi value:%f,hits:%ld,all samples:%ld",(4.0)*hits/samplePoints,hits,samplePoints);
    return 0;
}

void* compute_pi(void* arg) {
    long* local_hits = (int*)arg;
    unsigned int seed = (unsigned int)time(NULL) ^ (pthread_self());
    for (int index = 0; index<points_per_thread; index++) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        if (x*x + y*y <= 1) {
            *(local_hits) = *local_hits + 1;
        }
    }
    return NULL;
}