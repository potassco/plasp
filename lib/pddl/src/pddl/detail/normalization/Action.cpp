#include <pddl/detail/normalization/Action.h>

#include <pddl/AST.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/normalization/Effect.h>
#include <pddl/detail/normalization/Precondition.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Action
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::ActionPointer normalize(ast::ActionPointer &&action, detail::NormalizationContext &normalizationContext)
{
	auto normalizedAction = std::make_unique<normalizedAST::Action>();

	normalizedAction->name = std::move(action->name);
	normalizedAction->parameters = std::move(action->parameters);

	if (action->precondition)
		normalizedAction->precondition = normalize(std::move(action->precondition.value()), normalizationContext);

	if (action->effect)
		normalizedAction->effect = normalize(std::move(action->effect.value()), normalizationContext);

	return normalizedAction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
