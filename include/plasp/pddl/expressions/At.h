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

		size_t timePoint() const;

		void setArgument(ExpressionPointer argument);
		ExpressionPointer argument() const;

		ExpressionPointer normalized() override;

	protected:
		size_t m_timePoint;

		ExpressionPointer m_argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
AtPointer At::parse(Context &context, ExpressionContext &expressionContext,
	ExpressionParser parseExpression)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.testAndSkip<std::string>("(")
		|| !parser.testIdentifierAndSkip("at"))
	{
		parser.seek(position);
		return nullptr;
	}

	size_t timePoint;

	const auto timePointPosition = parser.position();

	if (parser.testIdentifierAndSkip("start"))
		timePoint = TimePointStart;
	else if (parser.testIdentifierAndSkip("end"))
		timePoint = TimePointEnd;
	else if (parser.probeNumber())
	{
		parser.seek(timePointPosition);
		timePoint = parser.parse<size_t>();
	}
	else
	{
		parser.seek(position);
		return nullptr;
	}

	auto expression = AtPointer(new At);

	expression->m_timePoint = timePoint;

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
