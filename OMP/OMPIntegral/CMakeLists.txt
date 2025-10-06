cmake_minimum_required(VERSION 3.28)
project(OMPIntegral C)

set(CMAKE_C_STANDARD 11)

find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME} main.c)

if(OpenMP_C_FOUND)
    message(STATUS "OpenMP found: ${OpenMP_C_FLAGS}")
    target_compile_options(${PROJECT_NAME} PUBLIC ${OpenMP_C_FLAGS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenMP_C_LIBRARIES} m)
endif()
