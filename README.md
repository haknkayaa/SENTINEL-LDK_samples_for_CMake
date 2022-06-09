
# SENTINEL LDK SAMPLES for CMAKE
[English](./README.md), [Turkish](./README.tr.md)

This repository includes SENTINEL LDK samples of Thales company developed for license protection.
It includes CMake-based revisions with the C and C++ programming language.

## Why?
The original examples are very primitive with gcc or makefiles
has been made. Because I'm using CMake in my modern Qt based project
reuse SENTINEL LDK API for CMake I had to revise.

Note: I do not know that I have expertise for SENTINEL LDK products and that I have any
I declare that I do not accept responsibility for the problem. More if you are an amateur
For professional support, please contact the company.

## Tested development environment:
- Ubuntu 20.04 LTS
- cmake version 3.16.3
- gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
- g++ (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
- SENTINEL LDK 8.42 for Linux

## How to use?

Open terminal and do like this:

- Go to the source folder.

  `cd ./Samples/Runtime/C-CMake/`

- Create a folder for Build and navigate into it.

  `mkdir build && cd build`

- Create a build file with CMake.

  `cmake ..`

- Run the `make` command for build.

  `make -jN (N is core count)`


## Folder structure:

- Samples/
  - [Activation/](./Samples/Activation/)
    - [Java](./Samples/Activation/Java/)
  - [Admin/](./Samples/Admin/)
    - [C](./Samples/Admin/C/)
    - [csharp](./Samples/Admin/csharp/)
  - [Envelope/](./Samples/Envelope/)
    - [Java](./Samples/Envelope/Java/)
    - [Python](./Samples/Envelope/Python/)
  - [Licgen/](./Samples/Licgen/)
    - [C](./Samples/Licgen/C/)
    - **[_C-CMake (my sample)_](./Samples/Licgen/C-CMake)**
  - [Runtime/](./Samples/Runtime/)
    - [C](./Samples/Runtime/C/)
    - [_**C-CMake (my sample)**_](./Samples/Runtime/C-CMake/)
    - [Cpp](./Samples/Runtime/Cpp/)
    - [**_Cpp-CMake (my sample)_**](./Samples/Runtime/Cpp-CMake/)
    - [csharp ](./Samples/Runtime/csharp/)
    - [Java](./Samples/Runtime/Java/)