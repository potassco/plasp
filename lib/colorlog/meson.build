colorlog_dependencies = [
	tokenize_dependency,
]

colorlog_include_directories = [
	include_directories('include'),
]

colorlog_sources = [
	'src/colorlog/Logger.cpp',
]

colorlog_library = static_library(
	'colorlog',
	dependencies: colorlog_dependencies,
	include_directories: colorlog_include_directories,
	sources: colorlog_sources,
)

colorlog_dependency = declare_dependency(
	link_with: colorlog_library,
	dependencies: colorlog_dependencies,
	include_directories: colorlog_include_directories,
)
