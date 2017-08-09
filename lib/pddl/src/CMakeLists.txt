set(target pddl)

file(GLOB core_sources "pddl/*.cpp")
file(GLOB core_headers "../include/pddl/*.h")

file(GLOB detail_sources "pddl/detail/*.cpp")
file(GLOB detail_headers "../include/pddl/detail/*.h")

file(GLOB detail_parsing_sources "pddl/detail/parsing/*.cpp")
file(GLOB detail_parsing_headers "../include/pddl/detail/parsing/*.h")

file(GLOB detail_normalization_sources "pddl/detail/normalization/*.cpp")
file(GLOB detail_normalization_headers "../include/pddl/detail/normalization/*.h")

set(includes
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/../../lib/colorlog/include
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

	${detail_normalization_sources}
	${detail_normalization_headers}
)

set(libraries
)

add_library(${target} ${sources})
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})
