cmake_minimum_required(VERSION 3.2)
project(zutils)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(gtest_SOURCE_DIR lib/googletest)

add_subdirectory(lib/googletest)
add_subdirectory(net)
add_subdirectory(utils)
