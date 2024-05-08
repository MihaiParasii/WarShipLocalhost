cmake_minimum_required(VERSION 3.27)
project(server C)

set(CMAKE_C_STANDARD 23)

add_executable(server main.c
        func.c
        func.h
        ships.c
        ships.h
        define.h
)

if(WIN32)
       target_link_libraries(server win2_32)
endif ()
