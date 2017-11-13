cmake_minimum_required(VERSION 2.6)
project(pddl)

option(PDDL_BUILD_TESTS "Build unit tests" OFF)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-g ${CMAKE_CXX_FLAGS_DEBUG}")

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
	set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always ${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(src)
if(PDDL_BUILD_TESTS)
	add_subdirectory(tests)
endif(PDDL_BUILD_TESTS)
