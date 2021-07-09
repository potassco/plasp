set(target tokenize-tests)

file(GLOB core_sources "*.cpp")

add_executable(${target} ${core_sources})
target_link_libraries(${target} PRIVATE catch tokenize)

add_custom_target(run-tokenize-tests
	COMMAND ${CMAKE_BINARY_DIR}/bin/tokenize-tests --use-colour=yes
	DEPENDS ${target}
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)
