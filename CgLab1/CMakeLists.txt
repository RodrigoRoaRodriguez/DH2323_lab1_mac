cmake_minimum_required(VERSION 3.5)
project(CgLab1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CgLab1_SOURCE_DIR}/cmake")

set(SOURCE_FILES skeleton.cpp)
add_executable(CgLab1 ${SOURCE_FILES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(CgLab1 ${SDL2_LIBRARY})



