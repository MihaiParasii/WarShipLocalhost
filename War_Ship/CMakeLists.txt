cmake_minimum_required(VERSION 3.26)
project(Morskoi_Boi C)

set(CMAKE_C_STANDARD 11)

add_executable(Morskoi_Boi main.c
        func.c
        func.h
        ships.c
        ships.h
        defines.c
        defines.h)
