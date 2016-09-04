#ifndef __PLASP__PDDL__EXPRESSIONS__NOT_H
#define __PLASP__PDDL__EXPRESSIONS__NOT_H

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Not
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Not: public ExpressionCRTP<Not>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::Not;

		template<typename ExpressionParser>
		static NotPointer parse(Context &context, ExpressionContext &expressionContext,
			ExpressionParser parseExpression);

	public:
		Not();

		void setArgument(ExpressionPointer argument);
		ExpressionPointer argument() const;

		ExpressionPointer reduced() override;
		ExpressionPointer negationNormalized() override;

		void print(std::ostream &ostream) const override;

	protected:
		ExpressionPointer m_argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
NotPointer Not::parse(Context &context, ExpressionContext &expressionContext,
	ExpressionParser parseExpression)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.testAndSkip<std::string>("(")
		|| !parser.testIdentifierAndSkip("not"))
	{
		parser.seek(position);
		return nullptr;
	}

	auto expression = NotPointer(new Not);

	context.parser.skipWhiteSpace();

	// Parse argument
	expression->setArgument(parseExpression(context, expressionContext));

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
