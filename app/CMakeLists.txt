set(target plasp-app)

find_package(Boost 1.55.0 COMPONENTS program_options iostreams system REQUIRED)

file(GLOB core_sources "*.cpp")
file(GLOB core_headers "*.h")

set(includes
	${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/lib/tokenize/include
	${PROJECT_SOURCE_DIR}/lib/colorlog/include
	${PROJECT_SOURCE_DIR}/lib/variant/include
	${PROJECT_SOURCE_DIR}/lib/pddlparse/include
)

set(sources
	${core_sources}
	${core_headers}
)

set(libraries
	stdc++fs
	${Boost_LIBRARIES}
	plasp
)

add_executable(${target} ${sources})
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})
set_target_properties(${target} PROPERTIES OUTPUT_NAME plasp)
