//
// Created by Chenx on 2025/10/10.
//
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"
#define ROOT_PROCESS 0
const int TASKS_NUM = 16;
int main(int argc,char* argv[]) {
    MPI_Init(&argc,&argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int world_count;
    MPI_Comm_size(MPI_COMM_WORLD,&world_count);
    OMPI_DECLSPEC  int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                                 void *recvbuf, int recvcount,
                                 MPI_Datatype recvtype, MPI_Comm comm);
    int* send_buff = (int*)malloc(sizeof(int)*TASKS_NUM/world_count);
    int send_count = TASKS_NUM/world_count;
    int* recvbuf = NULL;
    recvbuf = (int*)malloc(sizeof(int) * TASKS_NUM);
    int recvcount = TASKS_NUM;
    for (int index = 0;index< TASKS_NUM/world_count;index++) {
        send_buff[index] = rank + index;
    }
    MPI_Allgather(send_buff,send_count,MPI_INT,recvbuf,send_count,MPI_INT,MPI_COMM_WORLD);
    for (int i = 0; i < TASKS_NUM; i++) {
        printf("Rank:%d,allgather data:%d\n",rank,recvbuf[i]);
    }
    MPI_Finalize();
}