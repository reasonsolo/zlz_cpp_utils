cmake_minimum_required(VERSION 3.2)
project(zutils)

# for cygwin/MinGW tool chains
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(gtest_SOURCE_DIR lib/googletest)

add_subdirectory(lib/googletest)
add_subdirectory(net)
add_subdirectory(utils)
