cmake_minimum_required(VERSION 3.6)
project(cos_new)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(cos_new ${SOURCE_FILES})