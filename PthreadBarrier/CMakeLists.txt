cmake_minimum_required(VERSION 3.28)
project(PthreadBarrier C)

set(CMAKE_C_STANDARD 11)

add_executable(PthreadBarrier main.c
        PthreadBarrier.c
        PthreadBarrier.h)
