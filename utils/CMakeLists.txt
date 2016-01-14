cmake_minimum_required(VERSION 3.2)
project(zutils)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(UT_BINARY zutil_ut)
set(gtest_SOURCE_DIR ../lib/googletest)
set(LINK_LIBARIES gtest gtest_main pthread)

set(LIB_NAME utils)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(include)

AUX_SOURCE_DIRECTORY(src SOURCE_FILES)
AUX_SOURCE_DIRECTORY(test UT_SOURCE_FILES)

add_executable(${UT_BINARY} ${UT_SOURCE_FILES})
add_library(${LIB_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${UT_BINARY} ${LINK_LIBARIES} ${LIB_NAME})