#include <plasp/pddl/Expression.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Or.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/Reference.h>
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
	utils::Parser &parser, Context &context, const expressions::Variables &parameters);
ExpressionPointer parseEffectBodyExpressionContent(const std::string &expressionIdentifier,
	utils::Parser &parser, Context &context, const expressions::Variables &parameters);
ExpressionPointer parsePredicate(utils::Parser &parser, Context &context,
	const expressions::Variables &parameters);

////////////////////////////////////////////////////////////////////////////////////////////////////

void throwUnsupported(const utils::Parser &parser, const std::string &expressionIdentifier)
{
	throw utils::ParserException(parser, "Expression type \"" + expressionIdentifier + "\" currently unsupported");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePreconditionExpression(utils::Parser &parser, Context &context,
	const expressions::Variables &parameters)
{
	parser.expect<std::string>("(");

	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
	{
		expression = expressions::And::parse(parser, context, parameters,
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
	const expressions::Variables &parameters)
{
	parser.expect<std::string>("(");

	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	auto expression = parseExpressionContent(expressionIdentifier, parser, context, parameters);

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseExpressionContent(const std::string &expressionIdentifier,
	utils::Parser &parser, Context &context, const expressions::Variables &parameters)
{
	parser.skipWhiteSpace();

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
		expression = expressions::And::parse(parser, context, parameters, parseExpression);
	else if (expressionIdentifier == "or")
		expression = expressions::Or::parse(parser, context, parameters, parseExpression);
	else if (expressionIdentifier == "not")
		expression = expressions::Not::parse(parser, context, parameters, parseExpression);
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
		const auto match = std::find_if(context.predicateDeclarations.cbegin(), context.predicateDeclarations.cend(),
			[&](const auto &predicate)
			{
				return predicate->name() == expressionIdentifier;
			});

		// If predicate exists, parse it
		if (match != context.predicateDeclarations.cend())
			expression = expressions::Predicate::parse(expressionIdentifier, parser, context, parameters);
		else
			throw utils::ParserException(parser, "Expression \"" + expressionIdentifier + "\" not allowed in this context");
	}

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectExpression(utils::Parser &parser, Context &context,
	const expressions::Variables &parameters)
{
	parser.expect<std::string>("(");

	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
		expression = expressions::And::parse(parser, context, parameters, parseEffectExpression);
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
	utils::Parser &parser, Context &context, const expressions::Variables &parameters)
{
	ExpressionPointer expression;

	if (expressionIdentifier == "not")
		expression = expressions::Not::parse(parser, context, parameters, parsePredicate);
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
		const auto match = std::find_if(context.predicateDeclarations.cbegin(), context.predicateDeclarations.cend(),
			[&](const auto &predicate)
			{
				return predicate->name() == expressionIdentifier;
			});

		// If predicate exists, parse it
		if (match != context.predicateDeclarations.cend())
			expression = expressions::Predicate::parse(expressionIdentifier, parser, context, parameters);
		else
			throw utils::ParserException(parser, "Expression \"" + expressionIdentifier + "\" not allowed in this context");
	}

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePredicate(utils::Parser &parser, Context &context,
	const expressions::Variables &parameters)
{
	parser.expect<std::string>("(");

	const auto predicateName = parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	// Check if predicate with that name exists
	const auto match = std::find_if(context.predicateDeclarations.cbegin(), context.predicateDeclarations.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == predicateName;
		});

	// If predicate exists, parse it
	if (match == context.predicateDeclarations.cend())
		throw utils::ParserException(parser, "Unknown predicate \"" + predicateName + "\"");

	expression = expressions::Predicate::parse(predicateName, parser, context, parameters);

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
