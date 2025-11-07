cmake_minimum_required(VERSION 3.10)
project(lab_2.1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(lab_2.1 main.c ./src/functions.c)