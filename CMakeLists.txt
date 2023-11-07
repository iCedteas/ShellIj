cmake_minimum_required(VERSION 3.20.2)
project(shellIj)

set(CMAKE_CXX_STANDARD 14)

add_executable(shellIj main.cpp
        ShellIj.cpp
        ShellIj.h)

target_link_libraries(shellIj wsock32 ws2_32)