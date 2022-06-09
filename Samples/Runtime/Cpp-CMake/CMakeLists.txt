cmake_minimum_required(VERSION 3.22)
project(hasp_demo)

set(CMAKE_CXX_STANDARD 14)

# you need edit path
set(HASP_C_DIR ../../../API/Runtime/C/x86_64)
set(HASP_CPP_DIR ../../../API/Runtime/Cpp/x86_64)
set(HASP_CPP_LIB libhasp_cpp_linux_x86_64.a)

include_directories(${HASP_C_DIR})
include_directories(${HASP_CPP_DIR})

link_directories(${HASP_C_DIR})
link_directories(${HASP_CPP_DIR})
link_directories(${HASP_CPP_LIB})

add_executable(hasp_demo
        src/hasp_demo.cpp
        src/errorprinter.cpp
        src/errorprinter.h
#        src/hasp_api.h
        src/hasp_api_cpp.h
        src/vendor_code.h
        )
target_link_libraries(hasp_demo
        ${HASP_CPP_LIB}
        libhasp_linux_x86_64_demo.a
        pthread
        )

add_executable(hasp_update
        src/hasp_update.cpp
        src/errorprinter.cpp
        src/errorprinter.h
        #        src/hasp_api.h
        src/hasp_api_cpp.h
        src/vendor_code.h
        )



target_link_libraries(hasp_update
        ${HASP_CPP_LIB}
        libhasp_linux_x86_64_demo.a
        pthread
        )