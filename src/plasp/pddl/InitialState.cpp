#include <plasp/pddl/InitialState.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/IO.h>
#include <plasp/pddl/Problem.h>
#include <plasp/pddl/expressions/At.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/pddl/expressions/Unsupported.h>

#include <tokenize/TokenizerException.h>

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
	auto &tokenizer = context.tokenizer;

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

			const auto position = tokenizer.position();

			tokenizer.expect<std::string>("(");

			const auto expressionIdentifierPosition = tokenizer.position();

			if (tokenizer.testIdentifierAndSkip("="))
			{
				tokenizer.seek(expressionIdentifierPosition);
				const auto expressionIdentifier = tokenizer.getIdentifier();

				tokenizer.seek(position);
				return expressions::Unsupported::parse(context);
			}

			tokenizer.seek(expressionIdentifierPosition);
			const auto expressionIdentifier = tokenizer.getIdentifier();

			tokenizer.seek(position);
			throw tokenize::TokenizerException(tokenizer.location(), "expression type “" + expressionIdentifier + "” unknown or not allowed in this context");
		};

	tokenizer.skipWhiteSpace();

	while (tokenizer.currentCharacter() != ')')
	{
		ExpressionPointer expression;

		if ((expression = parseInitialStateElement()))
			initialState->m_facts.emplace_back(std::move(expression));

		tokenizer.skipWhiteSpace();
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
