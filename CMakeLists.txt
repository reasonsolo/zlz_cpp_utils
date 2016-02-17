cmake_minimum_required(VERSION 3.2)
project(zutils)

#include(ExternalProject)
#ExternalProject_add(libevent
#    URL ${PROJECT_SOURCE_DIR}/external/libevent-master.zip
#    PREFIX ${PROJECT_SOURCE_DIR}/lib/libevent
#    INSTALL_COMMAND ""
#)

# for cygwin/MinGW tool chains
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(gtest_SOURCE_DIR lib/googletest)
set(libevent_SOURCE_DIR lib/libevent)

add_subdirectory(lib/libevent)
add_subdirectory(lib/googletest)
add_subdirectory(net)
add_subdirectory(base)
add_subdirectory(evhttp)
