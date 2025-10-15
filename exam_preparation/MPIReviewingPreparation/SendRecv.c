//
// Created by Chenx on 2025/10/15.
//
#include "mpi.h"
#include <stdio.h>
int main() {
    MPI_Init(NULL,NULL);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    if (rank == 0) {
        int send_data[5];
        for (int i = 0; i < 5; i++) {
            send_data[i] = i;
        }
        MPI_Send(send_data,5,MPI_INT,1,0,MPI_COMM_WORLD);
    } else if (rank == 1) {
        int recev_data[5];
        MPI_Status status;
        MPI_Recv(recev_data,5,MPI_INT,0,0,MPI_COMM_WORLD,&status);
        int num;
        MPI_Get_count(&status,MPI_INT,&num);
        for (int i = 0; i < 5; i++) {
            printf("rank:%d received data:%d\n",rank,recev_data[i]);
        }
        printf("receive data from:%d,tag:%d\n",status.MPI_SOURCE,status.MPI_TAG);
    } else {
        printf("rank:%d,do nothing\n",rank);
    }
    MPI_Finalize();
}