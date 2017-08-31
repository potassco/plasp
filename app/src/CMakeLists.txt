set(target plasp-app)

find_package(Boost 1.55.0 COMPONENTS program_options iostreams system REQUIRED)

file(GLOB core_sources "*.cpp")
file(GLOB core_headers "../include/*.h")

file(GLOB commands_sources "commands/*.cpp")
file(GLOB commands_headers "../include/commands/*.h")

set(includes
	${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/lib/tokenize/include
	${PROJECT_SOURCE_DIR}/lib/colorlog/include
	${PROJECT_SOURCE_DIR}/lib/variant/include
	${PROJECT_SOURCE_DIR}/lib/pddl/include
)

set(sources
	${core_sources}
	${core_headers}
	${commands_sources}
	${commands_headers}
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
