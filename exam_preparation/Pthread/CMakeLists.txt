cmake_minimum_required(VERSION 3.28)
project(PthreadPi C)

set(CMAKE_C_STANDARD 11)

add_executable(PthreadPi #main.c
        #Pthread_Onsumer_Producer.c
        #PthreadBarrier.c
        #PthreadBarrierMain.c
        #PthreadBarrier.h
        #ReadWriteLockPthread/PthreadReadWriteLock.c
        #ReadWriteLockPthread/PthreadReadWriteLock.h
        #ReadWriteLockPthread/PthreadReadWriteLockMain.c
        #PthreadExam/PthreadConsumer.c
        #PthreadExam/PthreadConsumer2.c
        PthreadExam/PthreadMsg.c)
