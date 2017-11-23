cmake_minimum_required(VERSION 3.8)
project(tcp_ip)

add_definitions(-std=gnu++0x)
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

add_library(client_lib STATIC ${CLIENT_SOURCE_FILES})
add_library(server_lib STATIC ${SERVER_SOURCE_FILES})

add_subdirectory(testing)

target_link_libraries(tcp_ip_client client_lib)
target_link_libraries(tcp_ip_server server_lib)
