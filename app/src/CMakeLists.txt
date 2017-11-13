set(target plasp-app)

file(GLOB core_sources "plasp-app/*.cpp")
file(GLOB core_headers "../include/plasp-app/*.h")

file(GLOB commands_sources "plasp-app/commands/*.cpp")
file(GLOB commands_headers "../include/plasp-app/commands/*.h")

set(includes
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/lib/tokenize/include
	${PROJECT_SOURCE_DIR}/lib/colorlog/include
	${PROJECT_SOURCE_DIR}/lib/variant/include
	${PROJECT_SOURCE_DIR}/lib/pddl/include
	${PROJECT_SOURCE_DIR}/lib/cxxopts/include
	${PROJECT_SOURCE_DIR}/app/include
)

set(sources
	${core_sources}
	${core_headers}
	${commands_sources}
	${commands_headers}
)

set(libraries
	stdc++fs
	plasp
)

add_executable(${target} ${sources})
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})
set_target_properties(${target} PROPERTIES OUTPUT_NAME plasp)
