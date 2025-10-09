//
// Created by Chenx on 2025/10/9.
//
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"
#define ROOT_PROCESS 0
void scatterv(int my_rank,int world_count);
int main(int argc,char* argv[]) {
    MPI_Init(&argc,&argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int world_count;
    MPI_Comm_size(MPI_COMM_WORLD,&world_count);
    scatterv(rank,world_count);
    return 0;
}

void scatterv(int my_rank,int world_count) {
    const int N = 15;
    const int ROOT_RANK = 0;
    int base_task_count = N / world_count;
    int reminder = N % world_count;
    int recv_count;
    if (my_rank < reminder) {
        recv_count = base_task_count + 1;
    } else {
        recv_count = base_task_count;
    }
    int *sendcounts = NULL;
    int *displs = NULL;
    int *send_buffer = NULL;
    if (my_rank == ROOT_RANK) {
        send_buffer = (int*) malloc(sizeof(int) * N);
        displs = (int*) malloc(sizeof(int) * world_count);
        sendcounts = (int*) malloc(sizeof(int) * world_count);
        for (int i = 0; i < N; i++) {
            send_buffer[i] = 100 * i;
        }
        int currentDispls = 0;
        for (int i = 0; i < world_count; i++) {
            if (i<reminder) {
                sendcounts[i] = base_task_count + 1;
            } else {
                sendcounts[i] = base_task_count;
            }
            displs[i] = currentDispls;
            currentDispls += sendcounts[i];
        }
    }
    int *recv_buffer = (int*)malloc(sizeof(int) * recv_count);
    MPI_Scatterv(send_buffer,sendcounts,displs,MPI_INT,recv_buffer,recv_count,MPI_INT,ROOT_RANK,MPI_COMM_WORLD);
    for (int i = 0; i < recv_count; i++) {
        printf("Process:%d,receive data:%d\n",my_rank,recv_buffer[i]);
    }
    MPI_Finalize();
}