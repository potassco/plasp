pddl_dependencies = [
	tokenize_dependency,
	colorlog_dependency,
]

pddl_include_directories = [
	include_directories('../variant/include'),
	include_directories('include'),
]

pddl_link_args = [
	'-lstdc++fs',
]

pddl_sources = [
	'src/pddl/detail/normalization/Action.cpp',
	'src/pddl/detail/normalization/AtomicFormula.cpp',
	'src/pddl/detail/normalization/ConditionalEffect.cpp',
	'src/pddl/detail/normalization/Description.cpp',
	'src/pddl/detail/normalization/Domain.cpp',
	'src/pddl/detail/normalization/Effect.cpp',
	'src/pddl/detail/normalization/Fact.cpp',
	'src/pddl/detail/normalization/InitialState.cpp',
	'src/pddl/detail/normalization/Literal.cpp',
	'src/pddl/detail/normalization/Precondition.cpp',
	'src/pddl/detail/normalization/Problem.cpp',
	'src/pddl/detail/normalization/Reduction.cpp',
	'src/pddl/detail/parsing/Action.cpp',
	'src/pddl/detail/parsing/AtomicFormula.cpp',
	'src/pddl/detail/parsing/Constant.cpp',
	'src/pddl/detail/parsing/ConstantDeclaration.cpp',
	'src/pddl/detail/parsing/Description.cpp',
	'src/pddl/detail/parsing/Domain.cpp',
	'src/pddl/detail/parsing/Effect.cpp',
	'src/pddl/detail/parsing/Fact.cpp',
	'src/pddl/detail/parsing/InitialState.cpp',
	'src/pddl/detail/parsing/Precondition.cpp',
	'src/pddl/detail/parsing/Predicate.cpp',
	'src/pddl/detail/parsing/PredicateDeclaration.cpp',
	'src/pddl/detail/parsing/PrimitiveType.cpp',
	'src/pddl/detail/parsing/PrimitiveTypeDeclaration.cpp',
	'src/pddl/detail/parsing/Problem.cpp',
	'src/pddl/detail/parsing/Requirement.cpp',
	'src/pddl/detail/parsing/Term.cpp',
	'src/pddl/detail/parsing/Type.cpp',
	'src/pddl/detail/parsing/Unsupported.cpp',
	'src/pddl/detail/parsing/Variable.cpp',
	'src/pddl/detail/parsing/VariableDeclaration.cpp',
	'src/pddl/detail/Requirements.cpp',
	'src/pddl/detail/SignatureMatching.cpp',
	'src/pddl/detail/VariableStack.cpp',
	'src/pddl/Parse.cpp',
]

pddl_library = static_library(
	'pddl',
	link_args: pddl_link_args,
	dependencies: pddl_dependencies,
	include_directories: pddl_include_directories,
	sources: pddl_sources,
)

pddl_dependency = declare_dependency(
	link_with: pddl_library,
	link_args: pddl_link_args,
	dependencies: pddl_dependencies,
	include_directories: pddl_include_directories,
)

pddl_tests_include_directories = [
	include_directories('../catch/include'),
]

pddl_tests_sources = [
	'tests/main.cpp',
	'tests/TestIssues.cpp',
	'tests/TestParserBasics.cpp',
	'tests/TestParserCorrectness.cpp',
	'tests/TestParserSuccess.cpp',
	'tests/TestSignatureMatching.cpp',
]

pddl_tests = executable(
	'pddl-tests',
	dependencies: pddl_dependency,
	include_directories: pddl_tests_include_directories,
	sources: pddl_tests_sources,
)

test(
	'pddl-tests',
	pddl_tests,
)
