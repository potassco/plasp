#include <plasp/pddl/Expression.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/AndExpression.h>
#include <plasp/pddl/expressions/OrExpression.h>
#include <plasp/pddl/expressions/PredicateExpression.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Expression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Expression> parseExpressionBody(const std::string &expressionIdentifier, utils::Parser &parser, Context &context, const Variables &parameters);

////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Expression> parsePreconditionExpression(utils::Parser &parser, Context &context, const Variables &parameters)
{
	parser.skipWhiteSpace();

	parser.expect<std::string>("(");

	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	std::cout << "Parsing precondition expression " << expressionIdentifier << std::endl;

	std::unique_ptr<Expression> expression;

	const auto throwUnsupported =
		[&]()
		{
			throw utils::ParserException(parser.row(), parser.column(), "Expression type \"" + expressionIdentifier + "\" unsupported");
		};

	if (expressionIdentifier == "and")
		expression = expressions::AndExpression::parse(parser, context, parameters, parsePreconditionExpression);
	else if (expressionIdentifier == "forall")
		throwUnsupported();
	else if (expressionIdentifier == "preference")
		throwUnsupported();
	else
		expression = parseExpressionBody(expressionIdentifier, parser, context, parameters);

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Expression> parseExpression(utils::Parser &parser, Context &context, const Variables &parameters)
{
	parser.skipWhiteSpace();

	parser.expect<std::string>("(");

	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	auto expression = parseExpressionBody(expressionIdentifier, parser, context, parameters);

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Expression> parseExpressionBody(const std::string &expressionIdentifier, utils::Parser &parser, Context &context, const Variables &parameters)
{
	parser.skipWhiteSpace();

	std::cout << "Parsing expression " << expressionIdentifier << std::endl;

	std::unique_ptr<Expression> expression;

	const auto throwUnsupported =
	[&]()
	{
		throw utils::ParserException(parser.row(), parser.column(), "Expression type \"" + expressionIdentifier + "\" unsupported");
	};

	if (expressionIdentifier == "and")
		expression = expressions::AndExpression::parse(parser, context, parameters, parseExpression);
	else if (expressionIdentifier == "or")
		expression = expressions::OrExpression::parse(parser, context, parameters, parseExpression);
	else if (expressionIdentifier == "not"
		|| expressionIdentifier == "imply"
		|| expressionIdentifier == "exists"
		|| expressionIdentifier == "forall"
		|| expressionIdentifier == "-"
		|| expressionIdentifier == "="
		|| expressionIdentifier == "*"
		|| expressionIdentifier == "+"
		|| expressionIdentifier == "-"
		|| expressionIdentifier == "/"
		|| expressionIdentifier == ">"
		|| expressionIdentifier == "<"
		|| expressionIdentifier == "="
		|| expressionIdentifier == ">="
		|| expressionIdentifier == "<=")
	{
		throwUnsupported();
	}
	else
	{
		// Check if predicate with that name exists
		const auto match = std::find_if(context.predicates.cbegin(), context.predicates.cend(),
			[&](const auto &predicate)
			{
				return predicate->name() == expressionIdentifier;
			});

		// If predicate exists, parse it
		if (match != context.predicates.cend())
			expression = expressions::PredicateExpression::parse(expressionIdentifier, parser, context, parameters);
		else
			throw utils::ParserException(parser.row(), parser.column(), "Undeclared expression \"" + expressionIdentifier + "\"");
	}

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
