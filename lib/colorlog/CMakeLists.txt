set(target colorlog)

file(GLOB core_sources "src/colorlog/*.cpp")
file(GLOB core_headers "include/colorlog/*.h")

set(sources
	${core_sources}
	${core_headers}
)

add_library(${target} ${sources})
target_include_directories(${target}
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
target_link_libraries(${target} PUBLIC tokenize)
