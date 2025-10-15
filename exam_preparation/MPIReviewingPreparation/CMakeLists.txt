cmake_minimum_required(VERSION 3.28)
project(MPIReviewing C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER mpicc)

add_executable(MPIReviewing main.c
        SendRecv.c
        NewCommunicator.c)
