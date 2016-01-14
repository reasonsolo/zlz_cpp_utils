cmake_minimum_required(VERSION 3.2)

set(UT_BINARY zutil_net_ut)
set(gtest_SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/googletest)
set(LINK_LIBARYIES pthread utils net)

include_directories(${PROJECT_SOURCE_DIR}/utils/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

#AUX_SOURCE_DIRECTORY(. SOURCE_FILES)
FILE(GLOB SOURCE_FILES  "*.cc" )

add_executable(${UT_BINARY} ${SOURCE_FILES})

target_link_libraries(${UT_BINARY} ${LINK_LIBARIES} ${LIB_NAME} gtest gtest_main )
