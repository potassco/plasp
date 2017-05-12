#ifndef __PLASP__PDDL__EXPRESSIONS__AT_H
#define __PLASP__PDDL__EXPRESSIONS__AT_H

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
// At
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class At: public ExpressionCRTP<At>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::At;

		template<typename ExpressionParser>
		static AtPointer parse(Context &context, ExpressionContext &expressionContext,
			ExpressionParser parseExpression);

		static const size_t TimePointStart = std::numeric_limits<size_t>::max();
		static const size_t TimePointEnd = std::numeric_limits<size_t>::max() - 1;

	public:
		At();

		ExpressionPointer copy() override;

		size_t timePoint() const;

		void setArgument(ExpressionPointer argument);
		ExpressionPointer argument() const;

		ExpressionPointer reduced() override;
		ExpressionPointer existentiallyQuantified() override;
		ExpressionPointer simplified() override;

		void collectParameters(std::set<VariablePointer> &parameters) override;

		void print(std::ostream &ostream) const override;

	protected:
		size_t m_timePoint;

		ExpressionPointer m_argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
AtPointer At::parse(Context &context, ExpressionContext &expressionContext,
	ExpressionParser parseExpression)
{
	auto &tokenizer = context.tokenizer;

	const auto position = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("(")
		|| !tokenizer.testIdentifierAndSkip("at"))
	{
		tokenizer.seek(position);
		return nullptr;
	}

	size_t timePoint;

	const auto timePointPosition = tokenizer.position();

	if (tokenizer.testIdentifierAndSkip("start"))
		timePoint = TimePointStart;
	else if (tokenizer.testIdentifierAndSkip("end"))
		timePoint = TimePointEnd;
	else if (tokenizer.probeNumber())
	{
		tokenizer.seek(timePointPosition);
		timePoint = tokenizer.get<size_t>();
	}
	else
	{
		tokenizer.seek(position);
		return nullptr;
	}

	auto expression = AtPointer(new At);

	expression->m_timePoint = timePoint;

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
