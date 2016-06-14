#include <plasp/pddl/InitialState.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/IO.h>
#include <plasp/pddl/Problem.h>
#include <plasp/pddl/expressions/At.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/pddl/expressions/Unsupported.h>
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

std::unique_ptr<InitialState> InitialState::parseDeclaration(Context &context,
	ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	auto initialState = std::make_unique<InitialState>(InitialState());

	const auto parseInitialStateElement =
		[&]() -> ExpressionPointer
		{
			ExpressionPointer expression;

			// TODO: do not allow negative initial state literals
			if ((expression = parseLiteral(context, expressionContext))
				|| (expression = expressions::At::parse(context, expressionContext, parseLiteral)))
			{
				return expression;
			}

			const auto position = parser.position();

			parser.expect<std::string>("(");

			const auto expressionIdentifierPosition = parser.position();

			if (parser.probeIdentifier("=", isIdentifier))
			{
				parser.seek(expressionIdentifierPosition);
				const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

				parser.seek(position);
				return expressions::Unsupported::parse(context);
			}

			parser.seek(expressionIdentifierPosition);
			const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

			parser.seek(position);
			throw utils::ParserException(parser, "expression type “" + expressionIdentifier + "” unknown or not allowed in this context");
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
