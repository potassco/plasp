#ifndef __PLASP__PDDL__EXPRESSION__OR_H
#define __PLASP__PDDL__EXPRESSION__OR_H

#include <plasp/pddl/expressions/NAry.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Or
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Or: public NAry
{
	public:
		template<typename ExpressionParser>
		static OrPointer parse(Context &context, const Variables &parameters,
			ExpressionParser parseExpression);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

	private:
		Or() = default;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
OrPointer Or::parse(Context &context, const Variables &parameters, ExpressionParser parseExpression)
{
	auto expression = std::make_unique<Or>(Or());

	expression->NAry::parse(context, parameters, parseExpression);

	if (expression->arguments().empty())
		throw ConsistencyException("\"or\" expressions should not be empty");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
