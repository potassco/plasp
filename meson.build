project(
	'plasp',
	'cpp',
	default_options : [
		'buildtype=release',
		'cpp_std=c++1z',
	],
	subproject_dir: 'lib',
)

add_global_arguments(
	'-Wall',
	'-Wextra',
	'-Wpedantic',
	'-Werror',
	language: 'cpp',
)

tokenize = subproject('tokenize')
colorlog = subproject('colorlog')
pddl = subproject('pddl')

make_include_directories = [
	tokenize.get_variable('include_directories'),
	colorlog.get_variable('include_directories'),
	pddl.get_variable('include_directories'),
]

include_directories = [
	include_directories('include'),
]

link_with = [
	colorlog.get_variable('library'),
	pddl.get_variable('library'),
]

link_args = [
	'-lstdc++fs',
]

sources = [
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

plasp = shared_library(
	'plasp',
	include_directories : [make_include_directories, include_directories],
	link_args: link_args,
	link_with: link_with,
	sources: sources,
)

app_include_directories = [
	include_directories('app/include'),
	include_directories('lib/cxxopts/include'),
]

app_link_with = [
	link_with,
	plasp,
]

app_sources = [
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
	include_directories: [make_include_directories, include_directories, app_include_directories],
	link_with: app_link_with,
	sources: app_sources,
)
