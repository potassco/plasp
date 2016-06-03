#include <plasp/pddl/Expression.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/AndExpression.h>
#include <plasp/pddl/expressions/NotExpression.h>
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

ExpressionPointer parseExpressionContent(const std::string &expressionIdentifier,
	utils::Parser &parser, Context &context, const expressions::VariableExpressions &parameters);
ExpressionPointer parseEffectBodyExpressionContent(const std::string &expressionIdentifier,
	utils::Parser &parser, Context &context, const expressions::VariableExpressions &parameters);
ExpressionPointer parsePredicateExpression(utils::Parser &parser, Context &context,
	const expressions::VariableExpressions &parameters);

////////////////////////////////////////////////////////////////////////////////////////////////////

void throwUnsupported(const utils::Parser &parser, const std::string &expressionIdentifier)
{
	throw utils::ParserException(parser.row(), parser.column(), "Expression type \"" + expressionIdentifier + "\" currently unsupported");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePreconditionExpression(utils::Parser &parser, Context &context,
	const expressions::VariableExpressions &parameters)
{
	parser.expect<std::string>("(");

	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
	{
		expression = expressions::AndExpression::parse(parser, context, parameters,
			parsePreconditionExpression);
	}
	else if (expressionIdentifier == "forall"
		|| expressionIdentifier == "preference")
	{
		throwUnsupported(parser, expressionIdentifier);
	}
	else
		expression = parseExpressionContent(expressionIdentifier, parser, context, parameters);

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseExpression(utils::Parser &parser, Context &context,
	const expressions::VariableExpressions &parameters)
{
	parser.expect<std::string>("(");

	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	auto expression = parseExpressionContent(expressionIdentifier, parser, context, parameters);

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseExpressionContent(const std::string &expressionIdentifier,
	utils::Parser &parser, Context &context, const expressions::VariableExpressions &parameters)
{
	parser.skipWhiteSpace();

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
		expression = expressions::AndExpression::parse(parser, context, parameters, parseExpression);
	else if (expressionIdentifier == "or")
		expression = expressions::OrExpression::parse(parser, context, parameters, parseExpression);
	else if (expressionIdentifier == "not")
		expression = expressions::NotExpression::parse(parser, context, parameters, parseExpression);
	else if (expressionIdentifier == "imply"
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
		throwUnsupported(parser, expressionIdentifier);
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
			throw utils::ParserException(parser.row(), parser.column(), "Expression \"" + expressionIdentifier + "\" not allowed in this context");
	}

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectExpression(utils::Parser &parser, Context &context,
	const expressions::VariableExpressions &parameters)
{
	parser.expect<std::string>("(");

	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
		expression = expressions::AndExpression::parse(parser, context, parameters, parseEffectExpression);
	else if (expressionIdentifier == "forall"
		|| expressionIdentifier == "when")
	{
		throwUnsupported(parser, expressionIdentifier);
	}
	else
		expression = parseEffectBodyExpressionContent(expressionIdentifier, parser, context, parameters);

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectBodyExpressionContent(const std::string &expressionIdentifier,
	utils::Parser &parser, Context &context, const expressions::VariableExpressions &parameters)
{
	ExpressionPointer expression;

	if (expressionIdentifier == "not")
		expression = expressions::NotExpression::parse(parser, context, parameters, parsePredicateExpression);
	else if (expressionIdentifier == "="
		|| expressionIdentifier == "assign"
		|| expressionIdentifier == "scale-up"
		|| expressionIdentifier == "scale-down"
		|| expressionIdentifier == "increase"
		|| expressionIdentifier == "decrease")
	{
		throwUnsupported(parser, expressionIdentifier);
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
			throw utils::ParserException(parser.row(), parser.column(), "Expression \"" + expressionIdentifier + "\" not allowed in this context");
	}

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePredicateExpression(utils::Parser &parser, Context &context,
	const expressions::VariableExpressions &parameters)
{
	parser.expect<std::string>("(");

	const auto predicateName = parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	// Check if predicate with that name exists
	const auto match = std::find_if(context.predicates.cbegin(), context.predicates.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == predicateName;
		});

	// If predicate exists, parse it
	if (match == context.predicates.cend())
		throw utils::ParserException(parser.row(), parser.column(), "Unknown predicate \"" + predicateName + "\"");

	expression = expressions::PredicateExpression::parse(predicateName, parser, context, parameters);

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
