cmake_minimum_required(VERSION 2.8)

set(LIB_NAME net)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

include_directories(${PROJECT_SOURCE_DIR}/base/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

AUX_SOURCE_DIRECTORY(src SOURCE_FILES)

add_library(${LIB_NAME} STATIC ${SOURCE_FILES})

add_subdirectory(test)
