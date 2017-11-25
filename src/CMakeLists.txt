project(tcp_ip)

set(HEADER_FILES
        common/CustomTCP.h
        common/Protocol.h
        common/StandardTCP.h)

set(SOURCE_FILES
        common/CustomTCP.cpp
        common/Protocol.cpp
        common/StandardTCP.cpp
        client/Client.cpp
        server/Server.cpp)

add_library(tcp_ip_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})