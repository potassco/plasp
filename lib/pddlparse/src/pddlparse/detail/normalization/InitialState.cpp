#include <pddlparse/detail/normalization/InitialState.h>

#include <pddlparse/AST.h>
#include <pddlparse/Exception.h>
#include <pddlparse/NormalizedAST.h>
#include <pddlparse/detail/normalization/Fact.h>

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
