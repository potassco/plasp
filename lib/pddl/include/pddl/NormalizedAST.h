#ifndef __PDDL__NORMALIZED_AST_H
#define __PDDL__NORMALIZED_AST_H

#include <limits>
#include <experimental/optional>
#include <set>
#include <type_traits>
#include <vector>

#include <pddl/NormalizedASTForward.h>

namespace pddl
{
namespace normalizedAST
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Normalized AST
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// PDDL Structure
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Action
{
	Action() = default;

	Action(const Action &other) = delete;
	Action &operator=(const Action &&other) = delete;
	Action(Action &&other) = default;
	Action &operator=(Action &&other) = default;

	std::string name;

	VariableDeclarations parameters;
	std::experimental::optional<Precondition> precondition;
	std::experimental::optional<Effect> effect;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Domain
{
	Domain() = default;

	Domain(const Domain &other) = delete;
	Domain &operator=(const Domain &&other) = delete;
	Domain(Domain &&other) = delete;
	Domain &operator=(Domain &&other) = delete;

	std::string name;
	PrimitiveTypeDeclarations types;
	ConstantDeclarations constants;
	PredicateDeclarations predicates;
	DerivedPredicateDeclarations derivedPredicates;
	Actions actions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct DerivedPredicate
{
	using Arguments = Terms;

	explicit DerivedPredicate(Arguments &&arguments, DerivedPredicateDeclaration *declaration)
	:	arguments{std::move(arguments)},
		declaration{declaration}
	{
	}

	DerivedPredicate(const DerivedPredicate &other) = delete;
	DerivedPredicate &operator=(const DerivedPredicate &&other) = delete;
	DerivedPredicate(DerivedPredicate &&other) = default;
	DerivedPredicate &operator=(DerivedPredicate &&other) = default;

	Arguments arguments;
	DerivedPredicateDeclaration *declaration;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct DerivedPredicateDeclaration
{
	explicit DerivedPredicateDeclaration() = default;

	DerivedPredicateDeclaration(const DerivedPredicateDeclaration &other) = delete;
	DerivedPredicateDeclaration &operator=(const DerivedPredicateDeclaration &&other) = delete;
	DerivedPredicateDeclaration(DerivedPredicateDeclaration &&other) = default;
	DerivedPredicateDeclaration &operator=(DerivedPredicateDeclaration &&other) = default;

	std::string name;

	std::vector<VariableDeclaration *> parameters;
	VariableDeclarations existentialParameters;
	std::experimental::optional<DerivedPredicatePrecondition> precondition;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct InitialState
{
	InitialState() = default;

	InitialState(const InitialState &other) = delete;
	InitialState &operator=(const InitialState &&other) = delete;
	InitialState(InitialState &&other) = default;
	InitialState &operator=(InitialState &&other) = default;

	Facts facts;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Problem
{
	Problem() = default;

	Problem(Domain *domain)
	:	domain{domain}
	{
	}

	Problem(const Problem &other) = delete;
	Problem &operator=(const Problem &&other) = delete;
	Problem(Problem &&other) = default;
	Problem &operator=(Problem &&other) = default;

	Domain *domain;
	std::string name;
	DerivedPredicateDeclarations derivedPredicates;
	ConstantDeclarations objects;
	InitialState initialState;
	std::experimental::optional<Goal> goal;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Description
{
	Description() = default;

	Description(const Description &other) = delete;
	Description &operator=(const Description &&other) = delete;
	Description(Description &&other) = default;
	Description &operator=(Description &&other) = default;

	DomainPointer domain;
	std::experimental::optional<ProblemPointer> problem;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
