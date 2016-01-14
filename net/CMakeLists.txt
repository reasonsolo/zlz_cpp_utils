cmake_minimum_required(VERSION 3.2)

set(LINK_LIBARYIES gtest gtest_main utils)

set(LIB_NAME net)

include_directories(${PROJECT_SOURCE_DIR}/utils/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

AUX_SOURCE_DIRECTORY(src SOURCE_FILES)

add_library(${LIB_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${LIB_NAME} ${LINK_LIBARIES})

add_subdirectory(test)
