set(target tokenize-tests)

file(GLOB core_sources "*.cpp")

set(includes
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/../../lib/catch/single_include
)

set(libraries
	tokenize
)

add_executable(${target} ${core_sources})
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})

add_custom_target(run-tokenize-tests
	COMMAND ${CMAKE_BINARY_DIR}/bin/tokenize-tests
	DEPENDS ${target}
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)
