#include <plasp/pddl/InitialState.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/IO.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InitialState
//
////////////////////////////////////////////////////////////////////////////////////////////////////

inline void warnUnsupported(Context &context, const std::string &expressionIdentifier)
{
	context.logger.parserWarning(context.parser, "Expression type \"" + expressionIdentifier + "\" currently unsupported in this context");
	skipSection(context.parser);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<InitialState> InitialState::parseDeclaration(Context &context, const Problem &problem)
{
	auto initialState = std::make_unique<InitialState>(InitialState());

	while (context.parser.currentCharacter() != ')')
	{
		context.parser.expect<std::string>("(");

		ExpressionPointer expression;

		const auto expressionIdentifier = context.parser.parseIdentifier(isIdentifier);

		if (expressionIdentifier == "at"
			|| expressionIdentifier == "="
			|| expressionIdentifier == "not")
		{
			warnUnsupported(context, expressionIdentifier);

			continue;
		}

		// If none of the above types apply, the content is a predicate over constants and objects
		initialState->m_facts.emplace_back(expressions::Predicate::parse(expressionIdentifier, context, problem));

		context.parser.expect<std::string>(")");

		context.parser.skipWhiteSpace();
	}

	return initialState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
