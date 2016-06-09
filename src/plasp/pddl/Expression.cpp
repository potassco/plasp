#include <plasp/pddl/Expression.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/IO.h>
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

ExpressionPointer parseEffectBodyExpression(Context &context, ExpressionContext &expressionContext);
ExpressionPointer parsePredicate(Context &context, ExpressionContext &expressionContext);

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void warnUnsupported(Context &context, const std::string &expressionIdentifier)
{
	context.logger.parserWarning(context.parser, "Expression type \"" + expressionIdentifier + "\" currently unsupported in this context");
	skipSection(context.parser);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePreconditionExpression(Context &context,
	ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	parser.skipWhiteSpace();

	ExpressionPointer expression;

	if ((expression = expressions::And::parse(context, expressionContext, parsePreconditionExpression)))
		return expression;

	const auto position = parser.position();

	parser.expect<std::string>("(");

	const auto expressionIdentifierPosition = parser.position();

	if (parser.probeIdentifier("forall")
		|| parser.probeIdentifier("preference"))
	{
		// TODO: refactor
		parser.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

		parser.seek(position);
		warnUnsupported(context, expressionIdentifier);

		skipSection(parser);

		return nullptr;
	}

	parser.seek(position);
	return parseExpression(context, expressionContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseExpression(Context &context, ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	parser.skipWhiteSpace();

	ExpressionPointer expression;

	if ((expression = expressions::And::parse(context, expressionContext, parseExpression))
	    || (expression = expressions::Or::parse(context, expressionContext, parseExpression))
		|| (expression = expressions::Not::parse(context, expressionContext, parseExpression))
	    || (expression = expressions::Imply::parse(context, expressionContext, parseExpression))
	    || (expression = expressions::Predicate::parse(context, expressionContext)))
	{
		return expression;
	}

	const auto position = parser.position();

	parser.expect<std::string>("(");

	const auto expressionIdentifierPosition = parser.position();

	if (parser.probeIdentifier("exists")
		|| parser.probeIdentifier("forall")
		|| parser.probeIdentifier("-")
		|| parser.probeIdentifier("=")
		|| parser.probeIdentifier("*")
		|| parser.probeIdentifier("+")
		|| parser.probeIdentifier("-")
		|| parser.probeIdentifier("/")
		|| parser.probeIdentifier(">")
		|| parser.probeIdentifier("<")
		|| parser.probeIdentifier("=")
		|| parser.probeIdentifier(">=")
		|| parser.probeIdentifier("<="))
	{
		parser.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

		parser.seek(position);
		warnUnsupported(context, expressionIdentifier);

		skipSection(parser);

		return nullptr;
	}

	parser.seek(expressionIdentifierPosition);
	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	parser.seek(position);
	throw utils::ParserException(context.parser, "Expression type \"" + expressionIdentifier + "\" unknown or not allowed in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectExpression(Context &context, ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	ExpressionPointer expression;

	if ((expression = expressions::And::parse(context, expressionContext, parseEffectExpression)))
		return expression;

	const auto position = parser.position();

	parser.expect<std::string>("(");

	const auto expressionIdentifierPosition = parser.position();

	if (parser.probeIdentifier("forall")
		|| parser.probeIdentifier("when"))
	{
		parser.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

		parser.seek(position);
		warnUnsupported(context, expressionIdentifier);

		skipSection(parser);

		return nullptr;
	}

	parser.seek(position);
	return parseEffectBodyExpression(context, expressionContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectBodyExpression(Context &context, ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	ExpressionPointer expression;

	if ((expression = expressions::Not::parse(context, expressionContext, parsePredicate))
	    || (expression = expressions::Predicate::parse(context, expressionContext)))
	{
		return expression;
	}

	const auto position = parser.position();

	parser.expect<std::string>("(");

	const auto expressionIdentifierPosition = parser.position();

	if (parser.probeIdentifier("=")
		|| parser.probeIdentifier("assign")
		|| parser.probeIdentifier("scale-up")
		|| parser.probeIdentifier("scale-down")
		|| parser.probeIdentifier("increase")
		|| parser.probeIdentifier("decrease"))
	{
		parser.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

		parser.seek(position);
		warnUnsupported(context, expressionIdentifier);

		skipSection(parser);

		return nullptr;
	}

	parser.seek(expressionIdentifierPosition);
	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	parser.seek(position);
	throw utils::ParserException(context.parser, "Expression type \"" + expressionIdentifier + "\" unknown or not allowed in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePredicate(Context &context, ExpressionContext &expressionContext)
{
	ExpressionPointer expression;

	if ((expression = expressions::Predicate::parse(context, expressionContext)))
		return expression;

	throw utils::ParserException(context.parser, "Expected predicate");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
