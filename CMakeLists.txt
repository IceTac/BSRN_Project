cmake_minimum_required(VERSION 3.25)
project(BSRN_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Project_TCPClient TCPClient.c)
add_executable(Project_TCPServer TCPServer.c)

add_executable(Project_UDPClient UDPClient.c)
add_executable(Project_UDPServer UDPServer.c)