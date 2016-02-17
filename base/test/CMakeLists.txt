cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(UT_BINARY zutil_ut)
set(gtest_SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/googletest)
set(LINK_LIBARIES gtest gtest_main pthread base)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/base)

#AUX_SOURCE_DIRECTORY(. SOURCE_FILES)
FILE(GLOB SOURCE_FILES  "*.cc" )

add_executable(${UT_BINARY} ${SOURCE_FILES})

target_link_libraries(${UT_BINARY} ${LINK_LIBARIES})