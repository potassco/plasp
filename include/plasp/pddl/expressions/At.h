#ifndef __PLASP__PDDL__EXPRESSIONS__AT_H
#define __PLASP__PDDL__EXPRESSIONS__AT_H

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

		const Expression *argument() const;

	private:
		void setArgument(const Expression *argument);
		void setArgument(ExpressionPointer &&argument);

		size_t m_timePoint;

		const Expression *m_argument;
		ExpressionPointer m_argumentStorage;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
AtPointer At::parse(Context &context, ExpressionContext &expressionContext,
	ExpressionParser parseExpression)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.probe<std::string>("(")
		|| !parser.probeIdentifier("at", isIdentifier))
	{
		parser.seek(position);
		return nullptr;
	}

	size_t timePoint;

	const auto timePointPosition = parser.position();

	if (parser.probeIdentifier("start", isIdentifier))
		timePoint = TimePointStart;
	else if (parser.probeIdentifier("end", isIdentifier))
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

	auto expression = std::make_unique<At>(At());

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
