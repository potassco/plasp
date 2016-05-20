set(target plasp)

file(GLOB core_sources "plasp/*.cpp")
file(GLOB core_headers "../include/plasp/*.h")

file(GLOB sas_sources "plasp/sas/*.cpp")
file(GLOB sas_headers "../include/plasp/sas/*.h")

include_directories(
	${Boost_INCLUDE_DIRS}
)

include_directories(
	${PROJECT_SOURCE_DIR}/include
)

set(sources
	${core_sources}
	${core_headers}

	${sas_sources}
	${sas_headers}
)

set(libraries
	${Boost_LIBRARIES}
	pthread
)

add_library(${target} ${sources})
target_link_libraries(${target} ${libraries})
