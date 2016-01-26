cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

set(LIB_NAME utils)

include_directories(include)

AUX_SOURCE_DIRECTORY(src SOURCE_FILES)

add_library(${LIB_NAME} STATIC ${SOURCE_FILES})

add_subdirectory(test)
