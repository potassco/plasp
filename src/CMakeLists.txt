set(target plasp)

file(GLOB_RECURSE core_sources "*.cpp")
file(GLOB_RECURSE core_headers "../include/*.h")

set(sources
	${core_sources}
	${core_headers}
)

add_library(${target} ${sources})
target_include_directories(${target}
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
)
target_link_libraries(${target} PUBLIC Threads::Threads pddl)
