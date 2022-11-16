cmake_minimum_required(VERSION 3.1)
project(linux_msg)

set(CMAKE_CXX_STANDARD 14)

add_executable(linux_msg
        #disk.cpp
        #memory.cpp
        serverinfo_test.cpp
        server_info.cpp
        server_info.h
        #get_cpu_rate.cpp
        )
