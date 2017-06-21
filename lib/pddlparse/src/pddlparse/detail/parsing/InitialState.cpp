#include <pddlparse/detail/parsing/InitialState.h>

#include <pddlparse/AST.h>
#include <pddlparse/Exception.h>
#include <pddlparse/detail/parsing/Fact.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InitialState
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ast::InitialState parseInitialState(Context &context, ASTContext &astContext, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	ast::InitialState initialState;

	tokenizer.skipWhiteSpace();

	while (tokenizer.currentCharacter() != ')')
	{
		auto fact = parseFact(context, astContext, variableStack);

		if (!fact)
			throw ParserException(tokenizer.location(), "invalid initial state fact");

		initialState.facts.emplace_back(std::move(fact.value()));

		tokenizer.skipWhiteSpace();
	}

	return initialState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
