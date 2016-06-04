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
	Context &context, const expressions::Variables &parameters);
ExpressionPointer parseEffectBodyExpressionContent(const std::string &expressionIdentifier,
	Context &context, const expressions::Variables &parameters);
ExpressionPointer parsePredicate(Context &context, const expressions::Variables &parameters);

////////////////////////////////////////////////////////////////////////////////////////////////////

void throwUnsupported(const utils::Parser &parser, const std::string &expressionIdentifier)
{
	throw utils::ParserException(parser, "Expression type \"" + expressionIdentifier + "\" currently unsupported");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePreconditionExpression(Context &context,
	const expressions::Variables &parameters)
{
	context.parser.expect<std::string>("(");

	const auto expressionIdentifier = context.parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
	{
		expression = expressions::And::parse(context, parameters,
			parsePreconditionExpression);
	}
	else if (expressionIdentifier == "forall"
		|| expressionIdentifier == "preference")
	{
		throwUnsupported(context.parser, expressionIdentifier);
	}
	else
		expression = parseExpressionContent(expressionIdentifier, context, parameters);

	context.parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseExpression(Context &context, const expressions::Variables &parameters)
{
	context.parser.expect<std::string>("(");

	const auto expressionIdentifier = context.parser.parseIdentifier(isIdentifier);

	auto expression = parseExpressionContent(expressionIdentifier, context, parameters);

	context.parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseExpressionContent(const std::string &expressionIdentifier,
	Context &context, const expressions::Variables &parameters)
{
	context.parser.skipWhiteSpace();

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
		expression = expressions::And::parse(context, parameters, parseExpression);
	else if (expressionIdentifier == "or")
		expression = expressions::Or::parse(context, parameters, parseExpression);
	else if (expressionIdentifier == "not")
		expression = expressions::Not::parse(context, parameters, parseExpression);
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
		throwUnsupported(context.parser, expressionIdentifier);
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
			expression = expressions::Predicate::parse(expressionIdentifier, context, parameters);
		else
			throw utils::ParserException(context.parser, "Expression \"" + expressionIdentifier + "\" not allowed in this context");
	}

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectExpression(Context &context, const expressions::Variables &parameters)
{
	context.parser.expect<std::string>("(");

	const auto expressionIdentifier = context.parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
		expression = expressions::And::parse(context, parameters, parseEffectExpression);
	else if (expressionIdentifier == "forall"
		|| expressionIdentifier == "when")
	{
		throwUnsupported(context.parser, expressionIdentifier);
	}
	else
		expression = parseEffectBodyExpressionContent(expressionIdentifier, context, parameters);

	context.parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectBodyExpressionContent(const std::string &expressionIdentifier,
	Context &context, const expressions::Variables &parameters)
{
	ExpressionPointer expression;

	if (expressionIdentifier == "not")
		expression = expressions::Not::parse(context, parameters, parsePredicate);
	else if (expressionIdentifier == "="
		|| expressionIdentifier == "assign"
		|| expressionIdentifier == "scale-up"
		|| expressionIdentifier == "scale-down"
		|| expressionIdentifier == "increase"
		|| expressionIdentifier == "decrease")
	{
		throwUnsupported(context.parser, expressionIdentifier);
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
			expression = expressions::Predicate::parse(expressionIdentifier, context, parameters);
		else
			throw utils::ParserException(context.parser, "Expression \"" + expressionIdentifier + "\" not allowed in this context");
	}

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePredicate(Context &context, const expressions::Variables &parameters)
{
	context.parser.expect<std::string>("(");

	const auto predicateName = context.parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	// Check if predicate with that name exists
	const auto match = std::find_if(context.predicateDeclarations.cbegin(), context.predicateDeclarations.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == predicateName;
		});

	// If predicate exists, parse it
	if (match == context.predicateDeclarations.cend())
		throw utils::ParserException(context.parser, "Unknown predicate \"" + predicateName + "\"");

	expression = expressions::Predicate::parse(predicateName, context, parameters);

	context.parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
