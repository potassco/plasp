#include <pddlparse/detail/parsing/InitialState.h>

#include <pddlparse/AST.h>
#include <pddlparse/ParserException.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InitialState
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ast::InitialState parseInitialState(Context &context, ASTContext &)
{
	auto &tokenizer = context.tokenizer;

	ast::InitialState initialState;

	context.warningCallback(tokenizer.location(), "initial state parser under construction, section is currently ignored");

	// TODO: reimplement
	/*const auto parseInitialStateElement =
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
		ast::Expression expression;

		if ((expression = parseInitialStateElement()))
			initialState->m_facts.emplace_back(std::move(expression));

		tokenizer.skipWhiteSpace();
	}*/

	skipSection(tokenizer);

	return initialState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
