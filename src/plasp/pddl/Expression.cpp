#include <plasp/pddl/Expression.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Imply.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Or.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/Reference.h>
#include <plasp/utils/IO.h>
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

	const auto expressionIdentifier = utils::toLowerCase(context.parser.parseIdentifier(isIdentifier));

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

	const auto expressionIdentifier = utils::toLowerCase(context.parser.parseIdentifier(isIdentifier));

	auto expression = parseExpressionContent(expressionIdentifier, context, parameters);

	context.parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseExpressionContent(const std::string &expressionIdentifier,
	Context &context, const expressions::Variables &parameters)
{
	context.parser.skipWhiteSpace();

	if (expressionIdentifier == "and")
		return expressions::And::parse(context, parameters, parseExpression);

	if (expressionIdentifier == "or")
		return expressions::Or::parse(context, parameters, parseExpression);

	if (expressionIdentifier == "not")
		return expressions::Not::parse(context, parameters, parseExpression);

	if (expressionIdentifier == "imply")
		return expressions::Imply::parse(context, parameters, parseExpression);

	if (expressionIdentifier == "exists"
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

	// Check if predicate with that name exists
	const auto match = std::find_if(context.predicateDeclarations.cbegin(), context.predicateDeclarations.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == expressionIdentifier;
		});

	// If predicate exists, parse it
	if (match != context.predicateDeclarations.cend())
		return expressions::Predicate::parse(expressionIdentifier, context, parameters);

	throw utils::ParserException(context.parser, "Expression \"" + expressionIdentifier + "\" not allowed in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectExpression(Context &context, const expressions::Variables &parameters)
{
	context.parser.expect<std::string>("(");

	const auto expressionIdentifier = utils::toLowerCase(context.parser.parseIdentifier(isIdentifier));

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
		return expressions::Not::parse(context, parameters, parsePredicate);

	if (expressionIdentifier == "="
		|| expressionIdentifier == "assign"
		|| expressionIdentifier == "scale-up"
		|| expressionIdentifier == "scale-down"
		|| expressionIdentifier == "increase"
		|| expressionIdentifier == "decrease")
	{
		throwUnsupported(context.parser, expressionIdentifier);
	}

	// Check if predicate with that name exists
	const auto match = std::find_if(context.predicateDeclarations.cbegin(), context.predicateDeclarations.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == expressionIdentifier;
		});

	// If predicate exists, parse it
	if (match != context.predicateDeclarations.cend())
		return expressions::Predicate::parse(expressionIdentifier, context, parameters);

	throw utils::ParserException(context.parser, "Expression \"" + expressionIdentifier + "\" not allowed in this context");
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
