set(target tests)

file(GLOB core_sources "*.cpp")

add_subdirectory(googletest)

include_directories(
	${gtest_SOURCE_DIR}/include
	${gtest_SOURCE_DIR}
	${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
)

set(libraries
	gtest_main
	gtest
	${Boost_LIBRARIES}
	plasp
)

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${target} ${core_sources})
target_link_libraries(${target} ${libraries})
add_test(tests ${core_sources})

add_custom_target(run-tests
	COMMAND ${CMAKE_BINARY_DIR}/bin/tests
	DEPENDS ${target}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
