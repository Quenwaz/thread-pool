cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(threadpool LANGUAGES C CXX)

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

option(THREADPOOL_BUILD_TESTS "whether to compile the test set." ON)

add_subdirectory(src)

if(THREADPOOL_BUILD_TESTS)
    enable_testing()
    find_package(GTest QUIET)
    if (NOT GTEST_FOUND)
        message(FATAL_ERROR "google test not found.")
    endif()
    add_subdirectory(tests)
endif(THREADPOOL_BUILD_TESTS)

