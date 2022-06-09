cmake_minimum_required(VERSION 3.0)
project(sample)

set(CMAKE_CXX_STANDARD 14)

# you need edit path
set(HASP_C_DIR ../../../API/Licgen/C/x86_64)
set(HASP_C_LIB libsntl_licgen_linux_x86_64.so)

include_directories(${HASP_C_DIR})
link_directories(${HASP_C_DIR})

add_executable(sample
        src/sample.c
        src/sntl_licgen.h
        src/sntl_vcode.h
        )
target_link_libraries(sample
        ${HASP_C_LIB}
        pthread
        )

