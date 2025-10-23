cmake_minimum_required(VERSION 3.28)
project(Again2 C)

set(CMAKE_C_STANDARD 11)
find_package(OpenMP REQUIRED)
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
        #Pthread_barrier/PthreadBarrier.c
        #Pthread_barrier/PthreadBarrierMain.c
        #Pthread_barrier/PthreadBarrier.h
        #ReadWriteLock/PthreadReadWriteLock.c
        #ReadWriteLock/PthreadReadWriteLock.h
        #ReadWriteLock/PthreadReadWriteLockMain.c
        #OpenMp/OpenMpInit.c
        #OpenMp/OpenMpTest1.c
        #OpenMp/OpenMpTest2.c
        #OpenMp/Fib.c
        OpenMp/Recursion.c
        OpenMp/OpenMpRecursion2.c
)
if(OpenMP_C_FOUND)
    message(STATUS "OpenMP found: ${OpenMP_C_FLAGS}")
    target_compile_options(${PROJECT_NAME} PUBLIC ${OpenMP_C_FLAGS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenMP_C_LIBRARIES} m)
endif()