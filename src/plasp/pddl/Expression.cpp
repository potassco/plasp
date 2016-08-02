#include <plasp/pddl/Expression.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/IO.h>
#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Imply.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Or.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/Unsupported.h>
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

	if (parser.testIdentifierAndSkip("forall")
		|| parser.testIdentifierAndSkip("preference"))
	{
		// TODO: refactor
		parser.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = parser.parseIdentifier();

		parser.seek(position);
		return expressions::Unsupported::parse(context);
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

	if (parser.testIdentifierAndSkip("exists")
		|| parser.testIdentifierAndSkip("forall")
		|| parser.testIdentifierAndSkip("-")
		|| parser.testIdentifierAndSkip("=")
		|| parser.testIdentifierAndSkip("*")
		|| parser.testIdentifierAndSkip("+")
		|| parser.testIdentifierAndSkip("-")
		|| parser.testIdentifierAndSkip("/")
		|| parser.testIdentifierAndSkip(">")
		|| parser.testIdentifierAndSkip("<")
		|| parser.testIdentifierAndSkip("=")
		|| parser.testIdentifierAndSkip(">=")
		|| parser.testIdentifierAndSkip("<="))
	{
		parser.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = parser.parseIdentifier();

		parser.seek(position);
		return expressions::Unsupported::parse(context);
	}

	parser.seek(expressionIdentifierPosition);
	const auto expressionIdentifier = parser.parseIdentifier();

	parser.seek(position);
	throw utils::ParserException(parser.coordinate(), "expression type “" + expressionIdentifier + "” unknown or not allowed in this context");
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

	if (parser.testIdentifierAndSkip("forall")
		|| parser.testIdentifierAndSkip("when"))
	{
		parser.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = parser.parseIdentifier();

		parser.seek(position);
		return expressions::Unsupported::parse(context);
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

	if (parser.testIdentifierAndSkip("=")
		|| parser.testIdentifierAndSkip("assign")
		|| parser.testIdentifierAndSkip("scale-up")
		|| parser.testIdentifierAndSkip("scale-down")
		|| parser.testIdentifierAndSkip("increase")
		|| parser.testIdentifierAndSkip("decrease"))
	{
		parser.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = parser.parseIdentifier();

		parser.seek(position);
		return expressions::Unsupported::parse(context);
	}

	parser.seek(expressionIdentifierPosition);
	const auto expressionIdentifier = parser.parseIdentifier();

	parser.seek(position);
	throw utils::ParserException(parser.coordinate(), "expression type “" + expressionIdentifier + "” unknown or not allowed in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePredicate(Context &context, ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	ExpressionPointer expression;

	if ((expression = expressions::Predicate::parse(context, expressionContext)))
		return expression;

	throw utils::ParserException(parser.coordinate(), "expected predicate");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseLiteral(Context &context, ExpressionContext &expressionContext)
{
	ExpressionPointer expression;

	if ((expression = parseAtomicFormula(context, expressionContext))
		|| (expression = expressions::Not::parse(context, expressionContext, parseAtomicFormula)))
	{
		return expression;
	}

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseAtomicFormula(Context &context, ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	ExpressionPointer expression;

	if ((expression = expressions::Predicate::parse(context, expressionContext)))
		return expression;

	const auto position = parser.position();

	if (!parser.testAndSkip<std::string>("("))
		return nullptr;

	const auto expressionIdentifierPosition = parser.position();

	if (parser.testIdentifierAndSkip("="))
	{
		parser.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = parser.parseIdentifier();

		parser.seek(position);
		return expressions::Unsupported::parse(context);
	}

	parser.seek(position);
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
