cmake_minimum_required(VERSION 3.23)
project(web_server)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(web_server
        server.cpp
        )
