set(target tokenize)

file(GLOB core_sources "tokenize/*.cpp")
file(GLOB core_headers "../include/tokenize/*.h")

set(includes
	${PROJECT_SOURCE_DIR}/include
)

set(sources
	${core_sources}
	${core_headers}
)

set(libraries
	stdc++fs
)

add_library(${target} ${sources})
target_include_directories(${target} PRIVATE ${includes})
target_link_libraries(${target} ${libraries})
