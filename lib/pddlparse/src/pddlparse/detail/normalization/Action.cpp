#include <pddlparse/detail/normalization/Action.h>

#include <pddlparse/AST.h>
#include <pddlparse/NormalizedAST.h>
#include <pddlparse/detail/normalization/Effect.h>
#include <pddlparse/detail/normalization/Precondition.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Action
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::ActionPointer normalize(ast::ActionPointer &&action, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
{
	auto normalizedAction = std::make_unique<normalizedAST::Action>();

	normalizedAction->name = std::move(action->name);
	normalizedAction->parameters = std::move(action->parameters);

	if (action->precondition)
		normalizedAction->precondition = normalize(std::move(action->precondition.value()), derivedPredicates);

	if (action->effect)
		normalizedAction->effect = normalize(std::move(action->effect.value()), derivedPredicates);

	return normalizedAction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
