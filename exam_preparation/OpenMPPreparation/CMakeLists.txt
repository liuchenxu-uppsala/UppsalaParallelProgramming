cmake_minimum_required(VERSION 3.28)
project(OpenMPPreparation C)

set(CMAKE_C_STANDARD 11)

find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME} #main.c
        #Case1.c
        #AtomicCriticalReduction.c
        #fib/fib.c
        #exam/exam1.c
        #exam/exam2.c
        #exercise/exercise1.c
        #exercise/exercise2.c
        #exercise/exercise3.c
        #exercise/exercise4.c
        exercise/exercise4_2.c
        exercise/exercise5.c
)

if(OpenMP_C_FOUND)
    message(STATUS "OpenMP found: ${OpenMP_C_FLAGS}")
    target_compile_options(${PROJECT_NAME} PUBLIC ${OpenMP_C_FLAGS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenMP_C_LIBRARIES} m)
endif()
