#ifndef __PLASP__PDDL__EXPRESSIONS__BINARY_H
#define __PLASP__PDDL__EXPRESSIONS__BINARY_H

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/expressions/Variable.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Binary
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
class Binary: public Expression
{
	public:
		template<typename ExpressionParser>
		static std::unique_ptr<Derived> parse(Context &context,
			ExpressionContext &expressionContext, ExpressionParser parseExpression);

	public:
		const Expression *leftArgument() const;
		const Expression *rightArgument() const;

	private:
		ExpressionPointer m_leftArgument;
		ExpressionPointer m_rightArgument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
template<typename ExpressionParser>
std::unique_ptr<Derived> Binary<Derived>::parse(Context &context,
	ExpressionContext &expressionContext, ExpressionParser parseExpression)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.probe<std::string>("(")
		|| !parser.probeIdentifier(Derived::Identifier))
	{
		parser.seek(position);
		return nullptr;
	}

	auto expression = std::make_unique<Derived>();

	// Assume that expression identifier (imply, exists, etc.) is already parsed
	// Parse arguments of the expression
	expression->m_leftArgument = parseExpression(context, expressionContext);
	expression->m_rightArgument = parseExpression(context, expressionContext);

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
const Expression *Binary<Derived>::leftArgument() const
{
	return m_leftArgument.get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
const Expression *Binary<Derived>::rightArgument() const
{
	return m_rightArgument.get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
