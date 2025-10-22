cmake_minimum_required(VERSION 3.28)
project(Again2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Again2 #main.c
        #TASLock.c
        #TTASLock.c
        #BackLock.c
        #ALock.c
        #ALock2.c
        #ALock3.c
        #CLHLock.c
        #CLHLock2.c
        #MCSLock.c
        #PthreadPi.c
        #PthreadMessage.c
        #PthreadCustomerConsumer.c
        #PthreadProduceConsumerCondition.c
        Pthread_barrier/PthreadBarrier.c
        Pthread_barrier/PthreadBarrierMain.c
        Pthread_barrier/PthreadBarrier.h
)
target_link_libraries(Again2 m)