cmake_minimum_required(VERSION 3.27)
project(client C)

set(CMAKE_C_STANDARD 23)

add_executable(client main.c
        func.c
        func.h
        ships.c
        ships.h
        ../server/define.h
)


if(WIN32)
        target_link_libraries(client ws2_32)
endif ()