#ifndef __PLASP__PDDL__EXPRESSION__AND_H
#define __PLASP__PDDL__EXPRESSION__AND_H

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
		static AndPointer parse(Context &context, const Variables &parameters,
			ExpressionParser parseExpression);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

	private:
		And() = default;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
AndPointer And::parse(Context &context, const Variables &parameters,
	ExpressionParser parseExpression)
{
	auto expression = std::make_unique<And>(And());

	expression->NAry::parse(context, parameters, parseExpression);

	if (expression->arguments().empty())
		throw ConsistencyException("\"and\" expressions should not be empty");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
