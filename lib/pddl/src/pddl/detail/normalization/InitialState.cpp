#include <pddl/detail/normalization/InitialState.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/normalization/Fact.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InitialState
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::InitialState normalize(ast::InitialState &&initialState)
{
	normalizedAST::InitialState normalizedInitialState;
	normalizedInitialState.facts.reserve(initialState.facts.size());

	for (auto &fact : initialState.facts)
		normalizedInitialState.facts.emplace_back(normalize(std::move(fact)));

	return normalizedInitialState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
