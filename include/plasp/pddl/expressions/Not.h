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

		ExpressionPointer copy() override;

		void setArgument(ExpressionPointer argument);
		ExpressionPointer argument() const;

		ExpressionPointer reduced() override;
		ExpressionPointer existentiallyQuantified() override;
		ExpressionPointer simplified() override;
		ExpressionPointer decomposed(DerivedPredicates &derivedPredicates) override;

		void collectParameters(std::set<VariablePointer> &parameters) override;

		void print(std::ostream &ostream) const override;

	protected:
		ExpressionPointer m_argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
NotPointer Not::parse(Context &context, ExpressionContext &expressionContext,
	ExpressionParser parseExpression)
{
	auto &tokenizer = context.tokenizer;

	const auto position = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("(")
		|| !tokenizer.testIdentifierAndSkip("not"))
	{
		tokenizer.seek(position);
		return nullptr;
	}

	auto expression = NotPointer(new Not);

	tokenizer.skipWhiteSpace();

	// Parse argument
	expression->setArgument(parseExpression(context, expressionContext));

	tokenizer.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
