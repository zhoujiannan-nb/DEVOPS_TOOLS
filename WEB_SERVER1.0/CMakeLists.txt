cmake_minimum_required(VERSION 3.23)
project(web_server)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(web_server
        server.cpp
        tcp_socket.cpp
        tcp_socket.h
        tcp_server.h
        tcp_server.cpp
        ./threads_tools/Thread_pools.cpp
        ./threads_tools/Thread_pools.h
        ./threads_tools/Task_queue.cpp
        ./threads_tools/Task_queue.h
        )
