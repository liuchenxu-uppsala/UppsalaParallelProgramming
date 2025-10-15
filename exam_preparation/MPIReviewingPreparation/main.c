#include <stdio.h>
#include "mpi.h"
int main(void) {
    MPI_Init(NULL,NULL);
    // printf("Hello, World!\n");
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf("Rank:%d,all rank:%d\n",rank,size);
    MPI_Finalize();
    return 0;
}