cmake_minimum_required(VERSION 2.6)
project(plasp CXX)

option(PLASP_BUILD_TESTS "Build unit tests" OFF)
option(PLASP_BUILD_STATIC "Build static binaries" OFF)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-g ${CMAKE_CXX_FLAGS_DEBUG}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(PLASP_BUILD_STATIC)
	set(CMAKE_EXE_LINKER_FLAGS "-static")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)
	set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
endif()

if (CMAKE_GENERATOR STREQUAL "Ninja" AND
	((CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9) OR
	(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.5)))
	# Force colored warnings in Ninja's output, if the compiler has -fdiagnostics-color support.
	# Rationale in https://github.com/ninja-build/ninja/issues/814
	set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always ${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(lib/tokenize)
add_subdirectory(lib/colorlog)
add_subdirectory(lib/pddl)
add_subdirectory(src)
add_subdirectory(app)
if(PLASP_BUILD_TESTS)
	add_subdirectory(tests)
endif(PLASP_BUILD_TESTS)
