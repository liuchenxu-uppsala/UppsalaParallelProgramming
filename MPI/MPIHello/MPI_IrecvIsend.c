//
// Created by Chenx on 2025/10/9.
//

#include <stdio.h>
#include "mpi.h"
#define ROOT_PROCESS 0
int main(int argc,char* argv[]) {
    MPI_Init(&argc,&argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int partnerRank = 1 - rank;
    MPI_Request requests[2];
    int receiveData;
    MPI_Irecv(&receiveData,1,MPI_INT,partnerRank,99,MPI_COMM_WORLD,requests);
    MPI_Isend(&rank,1,MPI_INT,partnerRank,99,MPI_COMM_WORLD,requests+1);
    MPI_Waitall(2,requests,NULL);
    printf("RankId:%d,sendData:%d,receiveData:%d\n",rank,rank,receiveData);
    MPI_Finalize();
}