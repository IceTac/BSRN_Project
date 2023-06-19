cmake_minimum_required(VERSION 3.25)
project(BSRN_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Project_TCPClient TCP/TCPClient.c)
add_executable(Project_TCPServer TCP/TCPServer.c)

add_executable(Project_UDPClient UDP/UDPClient.c)
add_executable(Project_UDPServer UDP/UDPServer.c)

add_executable(Main main.c)