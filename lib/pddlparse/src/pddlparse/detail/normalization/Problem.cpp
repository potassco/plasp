#include <pddlparse/detail/normalization/Problem.h>

#include <pddlparse/AST.h>
#include <pddlparse/NormalizedAST.h>
#include <pddlparse/detail/normalization/InitialState.h>
#include <pddlparse/detail/normalization/Precondition.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Problem
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::ProblemPointer normalize(ast::ProblemPointer &&problem, normalizedAST::Domain *domain)
{
	auto normalizedProblem = std::make_unique<normalizedAST::Problem>(domain);

	normalizedProblem->name = std::move(problem->name);
	normalizedProblem->objects = std::move(problem->objects);
	normalizedProblem->initialState = normalize(std::move(problem->initialState));

	if (problem->goal)
		normalizedProblem->goal = normalize(std::move(problem->goal.value()), normalizedProblem->derivedPredicates);

	return normalizedProblem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
