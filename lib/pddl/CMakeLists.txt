set(target pddl)

option(PDDL_BUILD_TESTS "Build unit tests" OFF)

file(GLOB_RECURSE core_sources "src/pddl/*.cpp")
file(GLOB_RECURSE core_headers "include/pddl/*.h")

set(sources
	${core_sources}
	${core_headers}
)

add_library(${target} ${sources})
target_include_directories(${target}
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
target_link_libraries(${target}
	PUBLIC colorlog tokenize variant
)

if(PDDL_BUILD_TESTS)
	add_subdirectory(tests)
endif(PDDL_BUILD_TESTS)
