include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(tcp_ip_tests
        TestAll.cpp
        ClientTests.cpp)

target_link_libraries(tcp_ip_tests gtest gtest_main tcp_ip_lib)