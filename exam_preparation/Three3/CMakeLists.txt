cmake_minimum_required(VERSION 3.28)
project(Three3 C)

set(CMAKE_C_STANDARD 11)

add_executable(Three3 #main.c
        #Pthread/PthreadCustomerConsumer.c
        #Pthread/PthreadMessage.c
        #Pthread/PthreadProducerConsumerCondition.c
        #Pthread/PthreadBarrier/PthreadBarrier.c
        #Pthread/PthreadBarrier/PThreadBarrier.h
        #Pthread/PthreadBarrier/PThreadMain.c
        Pthread/PthreadReadWriteLock/PthreadReadWriteLock.c
        Pthread/PthreadReadWriteLock/PthreadReadWriteLock.h
        Pthread/PthreadReadWriteLock/PthreadReadWriteLockMain.c
)
