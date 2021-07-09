set(target tokenize)

option(TOKENIZE_BUILD_TESTS "Build unit tests" OFF)

file(GLOB core_headers "include/*.h")

set(sources
	${core_headers}
)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.19")
	add_library(${target} INTERFACE ${sources})
else()
	add_library(${target} INTERFACE)
endif()
target_include_directories(${target}
	INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

if(TOKENIZE_BUILD_TESTS)
	add_subdirectory(tests)
endif(TOKENIZE_BUILD_TESTS)
