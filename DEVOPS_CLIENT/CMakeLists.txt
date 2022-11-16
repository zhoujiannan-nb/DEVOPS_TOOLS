cmake_minimum_required(VERSION 3.23)
project(web_client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(web_client
        tcp_socket.cpp
        tcp_socket.h
        server_info.cpp
        server_info.h

        z_client.cpp)
