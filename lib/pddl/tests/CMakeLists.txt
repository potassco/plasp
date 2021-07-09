set(target pddl-tests)

file(GLOB core_sources "*.cpp")

add_executable(${target} ${core_sources})
target_include_directories(${target}
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
target_link_libraries(${target} PUBLIC std::filesystem pddl)

add_custom_target(run-pddl-tests
	COMMAND ${CMAKE_BINARY_DIR}/bin/pddl-tests --use-colour=yes
	DEPENDS ${target}
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../../tests)
