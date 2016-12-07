#include <plasp/pddl/Expression.h>

#include <plasp/input/ParserException.h>
#include <plasp/output/TranslatorException.h>
#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/IO.h>
#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Exists.h>
#include <plasp/pddl/expressions/ForAll.h>
#include <plasp/pddl/expressions/Imply.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Or.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/Unsupported.h>
#include <plasp/pddl/expressions/When.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Expression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Expression::copy()
{
	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Expression::normalized()
{
	return reduced()->simplified()->existentiallyQuantified()->simplified();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Expression::reduced()
{
	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Expression::existentiallyQuantified()
{
	return this;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Expression::moveUpQuantifiers(ExpressionPointer parent, ExpressionPointer &child)
{
	BOOST_ASSERT(child);

	if (child->expressionType() == Expression::Type::Exists)
	{
		auto quantifiedExpression = expressions::ExistsPointer(dynamic_cast<expressions::Exists *>(child.get()));

		// Move argument up
		child = quantifiedExpression->argument();

		// Move quantifier up
		quantifiedExpression->setArgument(parent);

		// Make parent point to the quantifier that has been moved up
		return quantifiedExpression;
	}
	else if (child->expressionType() == Expression::Type::ForAll)
	{
		auto quantifiedExpression = expressions::ForAllPointer(dynamic_cast<expressions::ForAll *>(child.get()));

		// Move argument up
		child = quantifiedExpression->argument();

		// Move quantifier up
		quantifiedExpression->setArgument(parent);

		// Make parent point to the quantifier that has been moved up
		return quantifiedExpression;
	}

	return parent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Expression::simplified()
{
	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

[[ noreturn ]] ExpressionPointer Expression::decomposed(expressions::DerivedPredicates &)
{
	throw output::TranslatorException("expression cannot be decomposed (not normalized)");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Expression::negated()
{
	if (expressionType() == Type::Not)
	{
		auto &notExpression = dynamic_cast<expressions::Not &>(*this);

		return notExpression.argument();
	}

	auto notExpression = expressions::NotPointer(new expressions::Not);
	notExpression->setArgument(this);

	return notExpression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: implement better (visitor pattern?)
void Expression::collectParameters(std::set<expressions::VariablePointer> &)
{
	throw output::TranslatorException("expression parameters could not be collected (expression not normalized)");
}

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

	if ((expression = expressions::And::parse(context, expressionContext, parsePreconditionExpression))
	    || (expression = expressions::ForAll::parse(context, expressionContext, parsePreconditionExpression)))
	{
		return expression;
	}

	const auto position = parser.position();

	parser.expect<std::string>("(");

	const auto expressionIdentifierPosition = parser.position();

	if (parser.testIdentifierAndSkip("preference"))
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
	    || (expression = expressions::Exists::parse(context, expressionContext, parseExpression))
	    || (expression = expressions::ForAll::parse(context, expressionContext, parseExpression))
		|| (expression = expressions::Not::parse(context, expressionContext, parseExpression))
	    || (expression = expressions::Imply::parse(context, expressionContext, parseExpression))
	    || (expression = expressions::Predicate::parse(context, expressionContext)))
	{
		return expression;
	}

	const auto position = parser.position();

	parser.expect<std::string>("(");

	const auto expressionIdentifierPosition = parser.position();

	if (parser.testIdentifierAndSkip("-")
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
	throw input::ParserException(parser.location(), "expression type “" + expressionIdentifier + "” unknown or not allowed in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseEffectExpression(Context &context, ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	ExpressionPointer expression;

	if ((expression = expressions::And::parse(context, expressionContext, parseEffectExpression))
	    || (expression = expressions::ForAll::parse(context, expressionContext, parseEffectExpression))
	    || (expression = expressions::When::parse(context, expressionContext, parseExpression, parseConditionalEffectExpression)))
	{
		return expression;
	}

	const auto position = parser.position();

	parser.expect<std::string>("(");

	const auto expressionIdentifierPosition = parser.position();

	if (parser.testIdentifierAndSkip("when"))
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
	throw input::ParserException(parser.location(), "expression type “" + expressionIdentifier + "” unknown or not allowed in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseConditionalEffectExpression(Context &context, ExpressionContext &expressionContext)
{
	ExpressionPointer expression;

	if ((expression = expressions::And::parse(context, expressionContext, parseEffectBodyExpression)))
		return expression;

	return parseEffectBodyExpression(context, expressionContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePredicate(Context &context, ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	ExpressionPointer expression;

	if ((expression = expressions::Predicate::parse(context, expressionContext)))
		return expression;

	throw input::ParserException(parser.location(), "expected predicate");
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
