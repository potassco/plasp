set(target pddlparse)

file(GLOB core_sources "pddlparse/*.cpp")
file(GLOB core_headers "../include/pddlparse/*.h")

file(GLOB detail_sources "pddlparse/detail/*.cpp")
file(GLOB detail_headers "../include/pddlparse/detail/*.h")

set(includes
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/../../lib/parsebase/include
)

set(sources
	${core_sources}
	${core_headers}

	${detail_sources}
	${detail_headers}
)

set(libraries
	parsebase
)

add_library(${target} ${sources})
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})
