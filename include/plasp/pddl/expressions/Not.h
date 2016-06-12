#ifndef __PLASP__PDDL__EXPRESSIONS__NOT_H
#define __PLASP__PDDL__EXPRESSIONS__NOT_H

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Identifier.h>

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

		const Expression *argument() const;

	private:
		void setArgument(const Expression *argument);
		void setArgument(ExpressionPointer &&argument);

		const Expression *m_argument;
		ExpressionPointer m_argumentStorage;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
NotPointer Not::parse(Context &context, ExpressionContext &expressionContext,
	ExpressionParser parseExpression)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.probe<std::string>("(")
		|| !parser.probeIdentifier("not", isIdentifier))
	{
		parser.seek(position);
		return nullptr;
	}

	auto expression = std::make_unique<Not>(Not());

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
