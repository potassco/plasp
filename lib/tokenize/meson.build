tokenize_include_directories = [
	include_directories('include'),
]

tokenize_dependency = declare_dependency(
	include_directories: tokenize_include_directories,
)

tokenize_tests_include_directories = [
	include_directories('../catch/include'),
]

tokenize_tests_sources = [
	'tests/main.cpp',
	'tests/TestTokenizer.cpp',
]

tokenize_tests = executable(
	'tokenize-tests',
	dependencies: tokenize_dependency,
	include_directories: tokenize_tests_include_directories,
	sources: tokenize_tests_sources,
)

test(
	'tokenize-tests',
	tokenize_tests,
	workdir: meson.source_root() + '/tests',
)
