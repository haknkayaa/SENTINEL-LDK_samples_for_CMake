cmake_minimum_required(VERSION 3.0)
project(hasp_demo)

set(CMAKE_CXX_STANDARD 14)

# you need edit path
set(HASP_C_DIR ../../../API/Runtime/C/x86_64)

include_directories(${HASP_C_DIR})
link_directories(${HASP_C_DIR})


add_executable(hasp_demo
        src/hasp_demo.c
        src/hasp_api.h
        src/hasp_vcode.h
        )
target_link_libraries(hasp_demo
        libhasp_linux_x86_64_demo.a
        pthread
        )

add_executable(hasp_update
        src/hasp_update.c
        src/hasp_api.h
        src/hasp_vcode.h
        )

target_link_libraries(hasp_update
        libhasp_linux_x86_64_demo.a
        pthread
        )