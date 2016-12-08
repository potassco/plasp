cmake_minimum_required(VERSION 2.6)
project(plasp CXX)

option(PLASP_BUILD_TESTS "Build unit tests" OFF)

find_package(Boost 1.55.0 COMPONENTS program_options iostreams system filesystem REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(app)
if(PLASP_BUILD_TESTS)
	add_subdirectory(tests)
endif(PLASP_BUILD_TESTS)
