cmake_minimum_required(VERSION 3.2)
project(zutils)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(test_BINARY zutil_ut)
set(gtest_SOURCE_DIR ../lib/googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(include)

set(SOURCE_FILES include/common.h include/string_utils.h include/histogram.h include/array_utils.h
        include/container_utils.h test/container_utils_ut.cc include/lock.h include/zthread.h src/zthread.cc src/lock.cc
        include/zfile.h include/time_utils.h include/zlog.h include/simple_log.h include/logger.h include/file_utils.h
        include/args_parser.h include/numeric_utils.h include/buffer.h include/ini_parser.h src/string_utils.cc test/string_utils_ut.cc include/singleton.h)
add_executable(${test_BINARY} ${SOURCE_FILES} src/numeric_utils.cc)

target_link_libraries(${test_BINARY} gtest gtest_main)