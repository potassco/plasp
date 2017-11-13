set(target pddl-tests)

file(GLOB core_sources "*.cpp")

set(includes
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/../../lib/catch/single_include
	${PROJECT_SOURCE_DIR}/../../lib/tokenize/include
	${PROJECT_SOURCE_DIR}/../../lib/variant/include
)

set(libraries
	stdc++fs
	pddl
)

add_executable(${target} ${core_sources})
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})

add_custom_target(run-pddl-tests
	COMMAND ${CMAKE_BINARY_DIR}/bin/pddl-tests --use-colour=yes
	DEPENDS ${target}
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../../tests)
