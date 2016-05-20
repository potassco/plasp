set(target tests)

file(GLOB core_sources "*.cpp")

find_package(GTest REQUIRED)

include_directories(
	${GTEST_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
)

set(libraries
	${Boost_LIBRARIES}
	${GTEST_BOTH_LIBRARIES}
	plasp
)

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${target} ${core_sources})
target_link_libraries(${target} ${libraries})
GTEST_ADD_TESTS(tests "" ${core_sources})

add_custom_target(run-tests
	COMMAND ${CMAKE_BINARY_DIR}/bin/tests
	DEPENDS ${target}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
