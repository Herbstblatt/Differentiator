cmake_minimum_required(VERSION 3.16)
project(Differentiator)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_BUILD_TYPE Debug)

include_directories(include
    external)

add_executable(differentiator src/main.cpp)