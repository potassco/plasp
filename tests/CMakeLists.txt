set(target tests)

file(GLOB core_sources "*.cpp")

add_executable(${target} ${core_sources})
target_include_directories(${target}
	PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
)
target_link_libraries(${target} PRIVATE catch std::filesystem plasp)

add_custom_target(run-tests
	COMMAND ${CMAKE_BINARY_DIR}/bin/tests --use-colour=yes
	DEPENDS ${target}
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)
