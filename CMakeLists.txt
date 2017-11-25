CMake_minimum_required(VERSION 3.8)
project(tcp_ip)

add_definitions(-std=c++11)
set(CMAKE_CXX_STANDARD 11)

set(CLIENT_SOURCE_FILES
        src/common/CustomTCP.cpp
        src/common/CustomTCP.h
        src/common/Protocol.cpp
        src/common/Protocol.h
        src/client/Client.cpp)

add_executable(tcp_ip_client ${CLIENT_SOURCE_FILES})

set(SERVER_SOURCE_FILES
        src/common/Protocol.cpp
        src/common/Protocol.h
        src/common/StandardTCP.cpp
        src/common/StandardTCP.h
        src/server/Server.cpp)

add_executable(tcp_ip_server ${SERVER_SOURCE_FILES})

include_directories(src src/common)
include_directories(~/.linuxbrew/Cellar/boost/1.65.1/include/)

add_subdirectory(testing)
add_subdirectory(src)


target_link_libraries(tcp_ip_client tcp_ip_lib)
target_link_libraries(tcp_ip_server tcp_ip_lib)
