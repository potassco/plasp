#ifndef __PLASP__PDDL__EXPRESSION__AND_EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION__AND_EXPRESSION_H

#include <plasp/pddl/expressions/NAryExpression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AndExpression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class AndExpression: public NAryExpression
{
	public:
		template<typename ExpressionParser>
		static AndExpressionPointer parse(utils::Parser &parser, Context &context,
			const VariableExpressions &parameters, ExpressionParser parseExpression);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

	private:
		AndExpression() = default;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
AndExpressionPointer AndExpression::parse(utils::Parser &parser, Context &context,
	const VariableExpressions &parameters, ExpressionParser parseExpression)
{
	auto expression = std::make_unique<AndExpression>(AndExpression());

	expression->NAryExpression::parse(parser, context, parameters, parseExpression);

	if (expression->arguments().empty())
		throw ConsistencyException("\"and\" expressions should not be empty");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
