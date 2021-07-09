set(target plasp-app)

file(GLOB_RECURSE core_sources "src/*.cpp")
file(GLOB_RECURSE core_headers "include/*.h")

set(sources
	${core_sources}
	${core_headers}
)

add_executable(${target} ${sources})
target_include_directories(${target}
	PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
target_link_libraries(${target} PRIVATE std::filesystem cxxopts plasp)
set_target_properties(${target} PROPERTIES OUTPUT_NAME plasp)
