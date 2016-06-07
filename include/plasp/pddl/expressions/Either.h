#ifndef __PLASP__PDDL__EXPRESSIONS__EITHER_H
#define __PLASP__PDDL__EXPRESSIONS__EITHER_H

#include <plasp/pddl/expressions/NAry.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Either
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Either: public NAry
{
	public:
		template<typename ExpressionParser>
		static EitherPointer parse(Context &context, ExpressionContext &expressionContext,
			ExpressionParser parseExpression);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

	private:
		Either() = default;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
EitherPointer Either::parse(Context &context, ExpressionContext &expressionContext,
	ExpressionParser parseExpression)
{
	auto expression = std::make_unique<Either>(Either());

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
