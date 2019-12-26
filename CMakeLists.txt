
cmake_minimum_required(VERSION 3.10.2)
project(libEventApp)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC
        )

add_executable(libEventApp main.cpp libEventApp.h ${SRC} customCallbacks.h)

target_link_libraries(libEventApp -levent)
