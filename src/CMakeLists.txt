set(target plasp)

file(GLOB core_sources "plasp/*.cpp")
file(GLOB core_headers "../include/plasp/*.h")

file(GLOB pddl_sources "plasp/pddl/*.cpp")
file(GLOB pddl_headers "../include/plasp/pddl/*.h")

file(GLOB pddl_expressions_sources "plasp/pddl/expressions/*.cpp")
file(GLOB pddl_expressions_headers "../include/plasp/pddl/expressions/*.h")

file(GLOB pddl_translation_sources "plasp/pddl/translation/*.cpp")
file(GLOB pddl_translation_headers "../include/plasp/pddl/translation/*.h")

file(GLOB sas_sources "plasp/sas/*.cpp")
file(GLOB sas_headers "../include/plasp/sas/*.h")

file(GLOB input_sources "plasp/input/*.cpp")
file(GLOB input_headers "../include/plasp/input/*.h")

file(GLOB output_sources "plasp/output/*.cpp")
file(GLOB output_headers "../include/plasp/output/*.h")

file(GLOB utils_sources "plasp/utils/*.cpp")
file(GLOB utils_headers "../include/plasp/utils/*.h")

set(includes
	${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/include
)

set(sources
	${core_sources}
	${core_headers}

	${pddl_sources}
	${pddl_headers}

	${pddl_expressions_sources}
	${pddl_expressions_headers}

	${pddl_translation_sources}
	${pddl_translation_headers}

	${sas_sources}
	${sas_headers}

	${input_sources}
	${input_headers}

	${output_sources}
	${output_headers}

	${utils_sources}
	${utils_headers}
)

set(libraries
	${Boost_LIBRARIES}
	pthread
)

add_library(${target} ${sources})
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})
