cmake_minimum_required(VERSION 3.28)
project(Three3 C)

set(CMAKE_C_STANDARD 11)
find_package(OpenMP REQUIRED)
add_executable(Three3 #main.c
        #Pthread/PthreadCustomerConsumer.c
        #Pthread/PthreadMessage.c
        #Pthread/PthreadProducerConsumerCondition.c
        #Pthread/PthreadBarrier/PthreadBarrier.c
        #Pthread/PthreadBarrier/PThreadBarrier.h
        #Pthread/PthreadBarrier/PThreadMain.c
        #Pthread/PthreadReadWriteLock/PthreadReadWriteLock.c
        #Pthread/PthreadReadWriteLock/PthreadReadWriteLock.h
        #Pthread/PthreadReadWriteLock/PthreadReadWriteLockMain.c
        #OpenMP/OpenMpTest1.c
        #OpenMP/SectionsMain.c
        #OpenMP/SingleMain.c
        #OpenMP/AtomicCriticalReduction.c
        #OpenMP/BarrierMain.c
        #OpenMP/Fib.c
        #OpenMP/RecurationExercise.c
        OpenMP/ThreadPrivate.c
)
if(OpenMP_C_FOUND)
    message(STATUS "OpenMP found: ${OpenMP_C_FLAGS}")
    target_compile_options(${PROJECT_NAME} PUBLIC ${OpenMP_C_FLAGS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenMP_C_LIBRARIES} m)
endif()
