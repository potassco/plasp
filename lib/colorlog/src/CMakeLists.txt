set(target colorlog)

file(GLOB core_sources "colorlog/*.cpp")
file(GLOB core_headers "../include/colorlog/*.h")

set(includes
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/../../lib/tokenize/include
)

set(sources
	${core_sources}
	${core_headers}
)

set(libraries
)

add_library(${target} ${sources})
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})
