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
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<InitialState> InitialState::parseDeclaration(Context &context, const Problem &problem)
{
	auto &parser = context.parser;

	auto initialState = std::make_unique<InitialState>(InitialState());

	const auto parseInitialStateElement =
		[&]()
		{
			ExpressionPointer expression;

			if ((expression = expressions::Predicate::parse(context, problem)))
				return expression;

			const auto position = parser.position();

			parser.expect<std::string>("(");

			const auto expressionIdentifierPosition = parser.position();

			if (parser.probeIdentifier("at", isIdentifier)
				|| parser.probeIdentifier("=", isIdentifier)
				|| parser.probeIdentifier("not", isIdentifier))
			{
				parser.seek(expressionIdentifierPosition);
				const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

				parser.seek(position);
				warnUnsupported(context, expressionIdentifier);

				parser.seek(expressionIdentifierPosition);
				skipSection(parser);

				return ExpressionPointer();
			}

			parser.seek(expressionIdentifierPosition);
			const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

			parser.seek(position);
			throw utils::ParserException(parser, "Expression type \"" + expressionIdentifier + "\" unknown or not allowed in this context");
		};

	parser.skipWhiteSpace();

	while (parser.currentCharacter() != ')')
	{
		ExpressionPointer expression;

		if ((expression = parseInitialStateElement()))
			initialState->m_facts.emplace_back(std::move(expression));

		parser.skipWhiteSpace();
	}

	return initialState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Expressions &InitialState::facts() const
{
	return m_facts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
