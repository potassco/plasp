cmake_minimum_required(VERSION 2.6)
project(plasp CXX)

find_package(Boost 1.55.0 COMPONENTS program_options iostreams system filesystem REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
add_definitions(-std=c++14)

option(BUILD_TESTS "Build unit tests" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(apps)
if(BUILD_TESTS)
	add_subdirectory(tests)
endif(BUILD_TESTS)
