
cmake_minimum_required(VERSION 3.10.2)
project(libEventApp)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC
        examples/BasicServer/main.cpp)

add_executable(libEventApp ${SRC})

target_link_libraries(libEventApp -levent)
