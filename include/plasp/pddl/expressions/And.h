#ifndef __PLASP__PDDL__EXPRESSIONS__AND_H
#define __PLASP__PDDL__EXPRESSIONS__AND_H

#include <plasp/pddl/expressions/NAry.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// And
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class And: public NAry
{
	public:
		template<typename ExpressionParser>
		static AndPointer parse(Context &context, ExpressionContext &expressionContext,
			ExpressionParser parseExpression);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

	private:
		And() = default;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
AndPointer And::parse(Context &context, ExpressionContext &expressionContext,
	ExpressionParser parseExpression)
{
	auto expression = std::make_unique<And>(And());

	expression->NAry::parse(context, expressionContext, parseExpression);

	if (expression->arguments().empty())
		throw ConsistencyException("\"and\" expressions should not be empty");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
