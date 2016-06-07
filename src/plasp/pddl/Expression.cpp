#include <plasp/pddl/Expression.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
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

ExpressionPointer parseExpressionContent(const std::string &expressionIdentifier, Context &context,
	ExpressionContext &expressionContext);
ExpressionPointer parseEffectBodyExpressionContent(const std::string &expressionIdentifier,
	Context &context, ExpressionContext &expressionContext);
ExpressionPointer parsePredicate(Context &context, ExpressionContext &expressionContext);

////////////////////////////////////////////////////////////////////////////////////////////////////

[[noreturn]] void throwUnsupported(const utils::Parser &parser,
	const std::string &expressionIdentifier)
{
	throw utils::ParserException(parser, "Expression type \"" + expressionIdentifier + "\" currently unsupported in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePreconditionExpression(Context &context,
	ExpressionContext &expressionContext)
{
	context.parser.expect<std::string>("(");

	const auto expressionIdentifier = context.parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
	{
		expression = expressions::And::parse(context, expressionContext,
			parsePreconditionExpression);
	}
	else if (expressionIdentifier == "forall"
		|| expressionIdentifier == "preference")
	{
		throwUnsupported(context.parser, expressionIdentifier);
	}
	else
		expression = parseExpressionContent(expressionIdentifier, context, expressionContext);

	context.parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseExpression(Context &context, ExpressionContext &expressionContext)
{
	context.parser.expect<std::string>("(");

	const auto expressionIdentifier = context.parser.parseIdentifier(isIdentifier);

	auto expression = parseExpressionContent(expressionIdentifier, context, expressionContext);

	context.parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseExpressionContent(const std::string &expressionIdentifier, Context &context,
	ExpressionContext &expressionContext)
{
	context.parser.skipWhiteSpace();

	if (expressionIdentifier == "and")
		return expressions::And::parse(context, expressionContext, parseExpression);

	if (expressionIdentifier == "or")
		return expressions::Or::parse(context, expressionContext, parseExpression);

	if (expressionIdentifier == "not")
		return expressions::Not::parse(context, expressionContext, parseExpression);

	if (expressionIdentifier == "imply")
		return expressions::Imply::parse(context, expressionContext, parseExpression);

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

	auto &predicateDeclarations = expressionContext.domain.predicates();

	// Check if predicate with that name exists
	const auto match = std::find_if(predicateDeclarations.cbegin(), predicateDeclarations.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == expressionIdentifier;
		});

	// If predicate exists, parse it
	if (match != predicateDeclarations.cend())
		return expressions::Predicate::parse(expressionIdentifier, context, expressionContext);

	throw utils::ParserException(context.parser, "Expression \"" + expressionIdentifier + "\" not allowed in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectExpression(Context &context, ExpressionContext &expressionContext)
{
	context.parser.expect<std::string>("(");

	const auto expressionIdentifier = context.parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	if (expressionIdentifier == "and")
		expression = expressions::And::parse(context, expressionContext, parseEffectExpression);
	else if (expressionIdentifier == "forall"
		|| expressionIdentifier == "when")
	{
		throwUnsupported(context.parser, expressionIdentifier);
	}
	else
		expression = parseEffectBodyExpressionContent(expressionIdentifier, context, expressionContext);

	context.parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectBodyExpressionContent(const std::string &expressionIdentifier,
	Context &context, ExpressionContext &expressionContext)
{
	ExpressionPointer expression;

	if (expressionIdentifier == "not")
		return expressions::Not::parse(context, expressionContext, parsePredicate);

	if (expressionIdentifier == "="
		|| expressionIdentifier == "assign"
		|| expressionIdentifier == "scale-up"
		|| expressionIdentifier == "scale-down"
		|| expressionIdentifier == "increase"
		|| expressionIdentifier == "decrease")
	{
		throwUnsupported(context.parser, expressionIdentifier);
	}

	const auto &predicateDeclarations = expressionContext.domain.predicates();

	// Check if predicate with that name exists
	const auto match = std::find_if(predicateDeclarations.cbegin(), predicateDeclarations.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == expressionIdentifier;
		});

	// If predicate exists, parse it
	if (match != predicateDeclarations.cend())
		return expressions::Predicate::parse(expressionIdentifier, context, expressionContext);

	throw utils::ParserException(context.parser, "Expression \"" + expressionIdentifier + "\" not allowed in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePredicate(Context &context, ExpressionContext &expressionContext)
{
	context.parser.expect<std::string>("(");

	const auto predicateName = context.parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	const auto &predicateDeclarations = expressionContext.domain.predicates();

	// Check if predicate with that name exists
	const auto match = std::find_if(predicateDeclarations.cbegin(), predicateDeclarations.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == predicateName;
		});

	// If predicate exists, parse it
	if (match == predicateDeclarations.cend())
		throw utils::ParserException(context.parser, "Unknown predicate \"" + predicateName + "\"");

	expression = expressions::Predicate::parse(predicateName, context, expressionContext);

	context.parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
