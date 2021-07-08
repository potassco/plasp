option(TOKENIZE_BUILD_TESTS "Build unit tests" OFF)
if(TOKENIZE_BUILD_TESTS)
	add_subdirectory(tests)
endif(TOKENIZE_BUILD_TESTS)
