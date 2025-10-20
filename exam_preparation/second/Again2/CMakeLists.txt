cmake_minimum_required(VERSION 3.28)
project(Again2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Again2 #main.c
        #TASLock.c
        #TTASLock.c
        BackLock.c)
target_link_libraries(Again2 m)