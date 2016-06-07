#ifndef __PLASP__PDDL__EXPRESSIONS__IMPLY_H
#define __PLASP__PDDL__EXPRESSIONS__IMPLY_H

#include <plasp/pddl/expressions/Binary.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Imply
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Imply: public Binary
{
	public:
		template<typename ExpressionParser>
		static ImplyPointer parse(Context &context, ExpressionContext &parameters,
			ExpressionParser parseExpression);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

	private:
		Imply() = default;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
ImplyPointer Imply::parse(Context &context, ExpressionContext &parameters,
	ExpressionParser parseExpression)
{
	auto expression = std::make_unique<Imply>(Imply());

	expression->Binary::parse(context, parameters, parseExpression);

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
