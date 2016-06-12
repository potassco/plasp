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
		const Expression &argument() const;

	private:
		ExpressionPointer m_argument;
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
	expression->m_argument = parseExpression(context, expressionContext);

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
