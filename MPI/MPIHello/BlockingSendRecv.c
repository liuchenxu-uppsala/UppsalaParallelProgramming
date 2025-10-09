//
// Created by Chenx on 2025/10/9.
//
#include <stdio.h>
#include "mpi.h"
int main(int argc,char* argv[]) {
    MPI_Init(&argc,&argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if (rank == 0) {
        int sendData[100];
        for (int i = 0; i < 100; i++) {
            sendData[i] = i;
        }
        int dataSize = 100;
        #define TAG_SEND_DATA_  0
        #define TAG_SEND_ARRA_DATA_  1
        MPI_Send(&dataSize, 1, MPI_INT, 1,TAG_SEND_DATA_, MPI_COMM_WORLD);
        MPI_Send(&dataSize, 1, MPI_INT, 2,TAG_SEND_DATA_, MPI_COMM_WORLD);

        MPI_Send(sendData, dataSize, MPI_INT, 1,TAG_SEND_ARRA_DATA_, MPI_COMM_WORLD);
        MPI_Send(sendData, dataSize, MPI_INT, 2,TAG_SEND_ARRA_DATA_, MPI_COMM_WORLD);
    } else {
        if (rank == 1 || rank ==2 ) {
            int dataSize;
            MPI_Recv(&dataSize,1,MPI_INT,0,TAG_SEND_DATA_,MPI_COMM_WORLD,NULL);
            printf("Rank:%d,data size:%d\n",rank,dataSize);
            int dataReceived[100];
            MPI_Recv(&dataReceived,dataSize,MPI_INT,0,TAG_SEND_ARRA_DATA_,MPI_COMM_WORLD,NULL);
            for (int i = 0; i < 100; i++) {
                printf("Rank:%d,receive data:%d\n",rank,dataReceived[i]);
            }
        }
    }
    MPI_Finalize();
    return 0;
}