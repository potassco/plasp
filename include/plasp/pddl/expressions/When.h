#ifndef __PLASP__PDDL__EXPRESSIONS__WHEN_H
#define __PLASP__PDDL__EXPRESSIONS__WHEN_H

#include <plasp/pddl/expressions/Binary.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// When
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class When: public Binary<When>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::When;

		static const std::string Identifier;

		template<typename ConditionExpressionParser, typename ImplicationExpressionParser>
		static WhenPointer parse(Context &context, ExpressionContext &expressionContext,
			ConditionExpressionParser parseConditionExpression,
			ImplicationExpressionParser parseImplicationExpression);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ConditionExpressionParser, typename ImplicationExpressionParser>
WhenPointer When::parse(Context &context, ExpressionContext &expressionContext,
	ConditionExpressionParser parseConditionExpression,
	ImplicationExpressionParser parseImplicationExpression)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.testAndSkip<std::string>("(")
		|| !parser.testIdentifierAndSkip(Identifier))
	{
		parser.seek(position);
		return nullptr;
	}

	auto expression = WhenPointer(new When);

	// Parse arguments of the expression
	expression->setArgument(0, parseConditionExpression(context, expressionContext));
	expression->setArgument(1, parseImplicationExpression(context, expressionContext));

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
