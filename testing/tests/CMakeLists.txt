include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(custom_tcp_ip_tests
        CustomTcpTests.cpp)

add_executable(standard_tcp_ip_tests
        StandardTcpTests.cpp)

target_link_libraries(custom_tcp_ip_tests gtest gtest_main tcp_ip_lib)
target_link_libraries(standard_tcp_ip_tests gtest gtest_main tcp_ip_lib)