cmake_minimum_required(VERSION 3.16)
project(Differentiator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
SET(CMAKE_BUILD_TYPE Debug)

include_directories(include
    external)

add_executable(differentiator src/main.cpp)
add_executable(test-file src/test.cpp)
add_custom_target(test
        ./test-file
        DEPENDS test-file)