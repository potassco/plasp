set(target tests)

file(GLOB core_sources "*.cpp")

include_directories(
	${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/lib/catch/single_include
)

set(libraries
	${Boost_LIBRARIES}
	plasp
)

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${target} ${core_sources})
target_link_libraries(${target} ${libraries})

add_custom_target(run-tests
	COMMAND ${CMAKE_BINARY_DIR}/bin/tests
	DEPENDS ${target}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
