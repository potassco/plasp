#ifndef __PLASP__PDDL__EXPRESSION__NOT_H
#define __PLASP__PDDL__EXPRESSION__NOT_H

#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Predicate.h>

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

class Not: public Expression
{
	public:
		template<typename ExpressionParser>
		static NotPointer parse(utils::Parser &parser, Context &context,
			const Variables &parameters, ExpressionParser parseExpression);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const Expression &argument();

	private:
		Not() = default;

		ExpressionPointer m_argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
NotPointer Not::parse(utils::Parser &parser, Context &context,
	const Variables &parameters, ExpressionParser parseExpression)
{
	auto expression = std::make_unique<Not>(Not());

	parser.skipWhiteSpace();

	// Parse argument
	expression->m_argument = parseExpression(parser, context, parameters);

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
