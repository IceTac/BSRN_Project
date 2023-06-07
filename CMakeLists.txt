cmake_minimum_required(VERSION 3.25)
project(BSRN_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Project_Client TCPClient.c)
add_executable(Project_Server TCPServer.c)