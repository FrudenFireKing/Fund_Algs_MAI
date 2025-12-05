cmake_minimum_required(VERSION 3.10)
project(lab_1.3 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(lab_1.3 ./src/main.c ./src/unqCheck.c ./src/triangleSidesCheck.c ./src/findRoot.c)

target_link_libraries(lab_1.3 m)