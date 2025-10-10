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
    // MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
    //                            void *recvbuf, int recvcount, MPI_Datatype recvtype,
    //                            int root, MPI_Comm comm);
    int* send_buff = NULL;
    if (rank == ROOT_PROCESS) {
        send_buff = (int*) malloc(sizeof(int) * TASKS_NUM);
        for (int index =0 ;index <TASKS_NUM; index++) {
            send_buff[index] = index;
        }
    }
    int sendcount = TASKS_NUM / world_count;
    int receive_buff[sendcount];
    MPI_Scatter(send_buff, sendcount, MPI_INT,
                               receive_buff, sendcount, MPI_INT,
                               ROOT_PROCESS, MPI_COMM_WORLD);
    for (int i = 0; i < sendcount; i++) {
        printf("Scatter value RankId:%d,receive scatter data:%d\n",rank,receive_buff[i]);
    }
    for (int i = 0; i < sendcount; i++) {
        receive_buff[i] = receive_buff[i] * receive_buff[i];
    }
    // OMPI_DECLSPEC  int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
    //                           void *recvbuf, int recvcount, MPI_Datatype recvtype,
    //                           int root, MPI_Comm comm);
    int* recvbuf_2 = NULL;
    if (rank == ROOT_PROCESS) {
        recvbuf_2 = (int*) malloc(sizeof(int) * TASKS_NUM);
    }
    MPI_Gather(receive_buff,sendcount,MPI_INT,recvbuf_2,sendcount,MPI_INT,ROOT_PROCESS,MPI_COMM_WORLD);
    if (rank == ROOT_PROCESS) {
        for (int i = 0; i < TASKS_NUM; i++) {
            printf("Gathered value RankId:%d,received data:%d\n",rank,recvbuf_2[i]);
        }
    }
    MPI_Finalize();
    return 0;
}