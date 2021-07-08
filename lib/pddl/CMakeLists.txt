option(PDDL_BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(src)
if(PDDL_BUILD_TESTS)
	add_subdirectory(tests)
endif(PDDL_BUILD_TESTS)
