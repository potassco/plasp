#ifndef __PLASP__PDDL__EXPRESSION__NOT_EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION__NOT_EXPRESSION_H

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
// NotExpression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class NotExpression: public Expression
{
	public:
		template<typename ExpressionParser>
		static std::unique_ptr<NotExpression> parse(utils::Parser &parser, Context &context, const Variables &parameters, ExpressionParser parseExpression);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const Expression &argument();

	private:
		NotExpression() = default;

		std::unique_ptr<Expression> m_argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
std::unique_ptr<NotExpression> NotExpression::parse(utils::Parser &parser, Context &context, const Variables &parameters, ExpressionParser parseExpression)
{
	auto expression = std::make_unique<NotExpression>(NotExpression());

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
