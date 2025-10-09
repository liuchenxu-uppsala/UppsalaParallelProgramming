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
    int world_count;
    MPI_Comm_size(MPI_COMM_WORLD,&world_count);
    #define TOTALTASKS 8
    #define TASKPERPROCESS TOTALTASKS/world_count
    #define ROOT 0
    int receive_buff[TASKPERPROCESS];
    int send_buff[TOTALTASKS];
    if (rank == ROOT) {
        for (int i = 0; i < TOTALTASKS; i++) {
            send_buff[i] = i *100;
        }
    }
    MPI_Scatter(send_buff,TASKPERPROCESS,MPI_INT,receive_buff,TASKPERPROCESS,MPI_INT,ROOT,MPI_COMM_WORLD);
    for (int i = 0; i < TASKPERPROCESS; i++) {
        printf("Process:%d,Receive Data:%d\n",rank,receive_buff[i]);
    }
    MPI_Finalize();
    return 0;
}