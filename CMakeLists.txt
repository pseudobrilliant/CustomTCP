cmake_minimum_required(VERSION 3.8)
project(TCP_IP)

set(CMAKE_CXX_STANDARD 11)

set(CLIENT_SOURCE_FILES
        src/common/CustomTCP.cpp
        src/common/CustomTCP.h
        src/common/Protocol.cpp
        src/common/Protocol.h
        src/client/Client.cpp)

add_executable(TCP_IP_Client ${CLIENT_SOURCE_FILES})

set(SERVER_SOURCE_FILES
        src/common/Protocol.cpp
        src/common/Protocol.h
        src/common/StandardTCP.cpp
        src/common/StandardTCP.h
        src/server/Server.cpp)

add_executable(TCP_IP_Server ${SERVER_SOURCE_FILES})