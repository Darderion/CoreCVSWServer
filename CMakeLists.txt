
cmake_minimum_required(VERSION 3.10.2)
project(libEventServer)

set(CMAKE_CXX_STANDARD 14)

function(cat IN_FILE OUT_FILE)
    file(READ ${IN_FILE} CONTENTS)
    file(APPEND ${OUT_FILE} "${CONTENTS}")
endfunction()

file(GLOB SRC main.cpp)
file(GLOB HTML
        Pages/html/Header.html
        Pages/css/style.css
        Pages/html/Body.html
        Pages/html/Content.html
        Pages/js/main.js
        Pages/html/Script.html)

# Prepare a temporary file to "cat" to:
file(WRITE Pages/index.template.html "")

# Call the "cat" function for each input file
foreach(html_template_file ${HTML})
    cat(${html_template_file} Pages/index.template.html)
endforeach()

# Copy the temporary file to the final location
configure_file(Pages/index.template.html Pages/index.html COPYONLY)

file(COPY
        Pages/img
        DESTINATION Pages/)

add_executable(coreCVSWServerApp ${SRC})

target_link_libraries(coreCVSWServerApp -levent)
