//
// Created by Chenx on 2025/10/15.
//
#include "mpi.h"
#include <stdio.h>
int main() {
    MPI_Init(NULL,NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    int color = world_rank % 3;
    MPI_Comm new_mpi_comm;
    MPI_Comm_split(MPI_COMM_WORLD,color,world_rank,&new_mpi_comm);
    int new_rank;
    MPI_Comm_rank(new_mpi_comm,&new_rank);
    printf("color:%d,old rank:%d,new rank:%d\n",color,world_rank,new_rank);
    MPI_Finalize();
    return 0;
}