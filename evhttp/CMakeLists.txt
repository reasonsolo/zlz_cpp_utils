cmake_minimum_required(VERSION 2.8)

# this project is aimed to building a cpp version of evhtp

set(LIB_NAME evhttp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(libevent_SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/libevent)

include_directories(${PROJECT_SOURCE_DIR}/base/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/libevent/include)
include_directories(${libevent_SOURCE_DIR}/include)
include_directories(${LIBEVENT_INCLUDE_DIRS})

AUX_SOURCE_DIRECTORY(src SOURCE_FILES)

add_library(${LIB_NAME} STATIC ${SOURCE_FILES})
#add_dependencies(${LIB_NAME} libevent)
add_subdirectory(test)
