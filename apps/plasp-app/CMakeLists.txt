set(target plasp_app)

file(GLOB core_sources "*.cpp")
file(GLOB core_headers "*.h")

include_directories(
	${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
)

set(sources
	${core_sources}
	${core_headers}
)

set(libraries
	${Boost_LIBRARIES}
	plasp
)

add_executable(${target} ${sources})
target_link_libraries(${target} ${libraries})
set_target_properties(plasp_app PROPERTIES OUTPUT_NAME plasp)
