#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int mum_threads;
long samplePoints;
long points_per_thread;
long total_hits = 0;
#define MAX_THREADS 512
pthread_t threads[MAX_THREADS];
int hits_per_thread[MAX_THREADS];

void* compute_pi(void* arg);
int main(void) {
    printf("How many points do you want to use?\n");
    scanf("%ld", &samplePoints);
    printf("How many threads do you want to use?\n");
    scanf("%d", &mum_threads);
    points_per_thread = samplePoints / mum_threads;
    srand((unsigned int)time(NULL));
    for (int i = 0; i < mum_threads; i++) {
        pthread_create(&threads[i], NULL, compute_pi, &hits_per_thread[i]);
    }
    for (int i = 0; i < mum_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < mum_threads; i++) {
        total_hits = total_hits + hits_per_thread[i];
    }
    printf("Pi is approximately %f,total_hits:%ld,samplePoints:%ld\n", 4.0 * total_hits / samplePoints, total_hits, samplePoints);
    return 0;
}

void* compute_pi(void* arg) {
    int* result  = (int*) arg;
    *result = 0;
    unsigned int seed = (unsigned int)time(NULL) ^ (pthread_self());
    for (int i = 0; i < points_per_thread; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double distance =  x * x + y * y;
        if (distance <= 1) {
            *result = *result + 1;
        }
    }
    return  NULL;
}