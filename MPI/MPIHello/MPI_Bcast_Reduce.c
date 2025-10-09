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
    long bcastLong=0;
    if (rank == ROOT_PROCESS) {
        bcastLong = 1000;
    }
    MPI_Bcast(&bcastLong,1,MPI_LONG,ROOT_PROCESS,MPI_COMM_WORLD);
    printf("ProcessId:%d,bcastLong:%ld\n",rank,bcastLong);
    int local_value = rank;
    int globalSum = 0;
    MPI_Reduce(&local_value,&globalSum,1,MPI_INT,MPI_SUM,ROOT_PROCESS,MPI_COMM_WORLD);
    if (rank == ROOT_PROCESS) {
        printf("ProcessId:%d,globalSum:%d\n",rank,globalSum);
    }
    MPI_Finalize();
    return 0;
}