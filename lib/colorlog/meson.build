project(
	'colorlog',
	'cpp',
	default_options : ['cpp_std=c++1z'],
)

tokenize = subproject('tokenize')

include_directories = [
	tokenize.get_variable('include_directories'),
	include_directories('include'),
]

sources = [
	'src/colorlog/Logger.cpp',
]

library = shared_library(
	'colorlog',
	include_directories: [include_directories],
	sources: sources,
)
