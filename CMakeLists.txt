
cmake_minimum_required(VERSION 3.10.2)
project(libEventServer)

set(CMAKE_CXX_STANDARD 14)

function(cat IN_FILE OUT_FILE)
    file(READ ${IN_FILE} CONTENTS)
    file(APPEND ${OUT_FILE} "${CONTENTS}")
endfunction()

file(GLOB SRC examples/BasicServer/main.cpp)
file(GLOB SRC2 examples/CoreCVS/main.cpp)
file(GLOB HTML
        examples/CoreCVS/Pages/html/Header.html
        examples/CoreCVS/Pages/css/style.css
        examples/CoreCVS/Pages/html/Body.html
        examples/CoreCVS/Pages/html/Content.html
        examples/CoreCVS/Pages/js/main.js
        examples/CoreCVS/Pages/html/Script.html)

# Prepare a temporary file to "cat" to:
file(WRITE examples/CoreCVS/Pages/index.template.html "")

# Call the "cat" function for each input file
foreach(html_template_file ${HTML})
    cat(${html_template_file} examples/CoreCVS/Pages/index.template.html)
endforeach()

# Copy the temporary file to the final location
configure_file(examples/CoreCVS/Pages/index.template.html Pages/index.html COPYONLY)

file(COPY
        examples/CoreCVS/Pages/img
        DESTINATION Pages/)

add_executable(libEventApp ${SRC})
add_executable(generateImageApp ${SRC2})

target_link_libraries(libEventApp -levent)
target_link_libraries(generateImageApp -levent)
