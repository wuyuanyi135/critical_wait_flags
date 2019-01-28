cmake_minimum_required(VERSION 3.13)
project(critical_wait_flags)

set(CMAKE_CXX_STANDARD 14)

add_executable(test test.cpp)
target_link_libraries(test pthread)