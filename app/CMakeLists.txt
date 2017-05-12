set(target plasp-app)

file(GLOB core_sources "*.cpp")
file(GLOB core_headers "*.h")

set(includes
	${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/lib/tokenize/include
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
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})
set_target_properties(${target} PROPERTIES OUTPUT_NAME plasp)
