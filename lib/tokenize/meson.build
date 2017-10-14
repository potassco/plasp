project(
	'tokenize',
	'cpp',
	default_options : ['cpp_std=c++1z'],
)

make_include_directories = [
	include_directories('../catch/include'),
]

include_directories = [
	include_directories('include'),
]

test_sources = [
	'tests/main.cpp',
	'tests/TestTokenizer.cpp',
]

tests = executable(
	'tokenize-tests',
	include_directories: [make_include_directories, include_directories],
	sources: test_sources,
)

test(
	'tokenize-tests',
	tests,
)
