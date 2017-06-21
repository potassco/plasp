set(target pddlparse)

file(GLOB core_sources "pddlparse/*.cpp")
file(GLOB core_headers "../include/pddlparse/*.h")

file(GLOB detail_sources "pddlparse/detail/*.cpp")
file(GLOB detail_headers "../include/pddlparse/detail/*.h")

file(GLOB detail_parsing_sources "pddlparse/detail/parsing/*.cpp")
file(GLOB detail_parsing_headers "../include/pddlparse/detail/parsing/*.h")

set(includes
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/../../lib/tokenize/include
	${PROJECT_SOURCE_DIR}/../../lib/variant/include
)

set(sources
	${core_sources}
	${core_headers}

	${detail_sources}
	${detail_headers}

	${detail_parsing_sources}
	${detail_parsing_headers}
)

set(libraries
)

add_library(${target} ${sources})
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})
