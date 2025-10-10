cmake_minimum_required(VERSION 3.28)
project(MPIHello C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER mpicc)

add_executable(MPIHello #main.c
        #BlockingSendRecv.c
        #MPI_Bcast_Reduce.c
        #MPI_IrecvIsend.c
        #Books/HiMPI.c
        #MPI_Scatter.c
        #Books/MPI_Scatterv.c
        #Books/MPI_ScatterAndGather.c
        Books/MPI_ALLGather.c)
