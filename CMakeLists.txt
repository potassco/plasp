cmake_minimum_required(VERSION 2.6)
project(plasp CXX)

option(PLASP_BUILD_TESTS "Build unit tests" OFF)

find_package(Boost 1.55.0 COMPONENTS program_options iostreams system REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (CMAKE_GENERATOR STREQUAL "Ninja" AND
	((CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9) OR
	(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.5)))
	# Force colored warnings in Ninja's output, if the compiler has -fdiagnostics-color support.
	# Rationale in https://github.com/ninja-build/ninja/issues/814
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
endif()

add_subdirectory(lib/tokenize)
add_subdirectory(lib/pddlparse)
add_subdirectory(src)
add_subdirectory(app)
if(PLASP_BUILD_TESTS)
	add_subdirectory(tests)
endif(PLASP_BUILD_TESTS)
