
cmake_minimum_required(VERSION 3.10.2)
project(libEventServer)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC examples/BasicServer/main.cpp)
file(GLOB SRC2 examples/CoreCVS/main.cpp)

file(COPY
        examples/CoreCVS/Pages/
        DESTINATION Pages)

add_executable(libEventApp ${SRC})
add_executable(generateImageApp ${SRC2})

target_link_libraries(libEventApp -levent)
target_link_libraries(generateImageApp -levent)
