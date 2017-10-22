project(
	'plasp',
	'cpp',
	default_options : [
		'buildtype=release',
		'warning_level=3',
		'werror=true',
		'cpp_std=c++1z',
	],
)

subdir('lib')

plasp_dependencies = [
	tokenize_dependency,
	colorlog_dependency,
	pddl_dependency,
]

plasp_include_directories = [
	include_directories('include'),
]

plasp_sources = [
	'src/plasp/Language.cpp',
	'src/plasp/pddl/TranslatorASP.cpp',
	'src/plasp/sas/AssignedVariable.cpp',
	'src/plasp/sas/AxiomRule.cpp',
	'src/plasp/sas/Debugging.cpp',
	'src/plasp/sas/Description.cpp',
	'src/plasp/sas/Effect.cpp',
	'src/plasp/sas/Goal.cpp',
	'src/plasp/sas/InitialState.cpp',
	'src/plasp/sas/MutexGroup.cpp',
	'src/plasp/sas/Operator.cpp',
	'src/plasp/sas/Predicate.cpp',
	'src/plasp/sas/TranslatorASP.cpp',
	'src/plasp/sas/Value.cpp',
	'src/plasp/sas/Variable.cpp',
	'src/plasp/sas/VariableTransition.cpp',
]

plasp_library = static_library(
	'plasp',
	dependencies: plasp_dependencies,
	include_directories : plasp_include_directories,
	sources: plasp_sources,
)

plasp_dependency = declare_dependency(
	link_with: plasp_library,
	dependencies: plasp_dependencies,
	include_directories: plasp_include_directories,
)

plasp_tests_include_directories = [
	include_directories('lib/catch/include'),
]

plasp_tests_sources = [
	'tests/main.cpp',
	'tests/TestPDDLNormalization.cpp',
	'tests/TestPDDLTranslation.cpp',
	'tests/TestSASParser.cpp',
]

plasp_tests = executable(
	'plasp-tests',
	dependencies: plasp_dependency,
	include_directories: plasp_tests_include_directories,
	sources: plasp_tests_sources,
)

test(
	'plasp-tests',
	plasp_tests,
	workdir: meson.source_root() + '/tests',
)

plasp_app_dependencies = [
	plasp_dependency,
]

plasp_app_include_directories = [
	include_directories('app/include'),
	include_directories('lib/cxxopts/include'),
]

plasp_app_sources = [
	'app/src/plasp-app/commands/CommandBeautify.cpp',
	'app/src/plasp-app/commands/CommandCheckSyntax.cpp',
	'app/src/plasp-app/commands/CommandHelp.cpp',
	'app/src/plasp-app/commands/CommandNormalize.cpp',
	'app/src/plasp-app/commands/CommandTranslate.cpp',
	'app/src/plasp-app/commands/CommandVersion.cpp',
	'app/src/plasp-app/main.cpp',
	'app/src/plasp-app/OptionGroups.cpp',
]

plasp_app = executable(
	'plasp',
	dependencies: plasp_app_dependencies,
	include_directories: plasp_app_include_directories,
	sources: plasp_app_sources,
)
