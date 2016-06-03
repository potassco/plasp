#ifndef __PLASP__PDDL__EXPRESSION__OR_EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION__OR_EXPRESSION_H

#include <plasp/pddl/expressions/NAryExpression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// OrExpression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class OrExpression: public NAryExpression
{
	public:
		template<typename ExpressionParser>
		static OrExpressionPointer parse(utils::Parser &parser, Context &context,
			const VariableExpressions &parameters, ExpressionParser parseExpression);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

	private:
		OrExpression() = default;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
OrExpressionPointer OrExpression::parse(utils::Parser &parser, Context &context,
	const VariableExpressions &parameters, ExpressionParser parseExpression)
{
	auto expression = std::make_unique<OrExpression>(OrExpression());

	expression->NAryExpression::parse(parser, context, parameters, parseExpression);

	if (expression->arguments().empty())
		throw ConsistencyException("\"or\" expressions should not be empty");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
