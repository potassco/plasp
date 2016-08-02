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
class Binary: public ExpressionCRTP<Derived>
{
	public:
		template<typename ExpressionParser>
		static std::unique_ptr<Derived> parse(Context &context,
			ExpressionContext &expressionContext, ExpressionParser parseExpression);

	public:
		const std::array<const Expression *, 2> &arguments() const;

	private:
		template<size_t i>
		void setArgument(const Expression *argument);
		template<size_t i>
		void setArgument(ExpressionPointer &&argument);

		std::array<const Expression *, 2> m_arguments;
		std::array<ExpressionPointer, 2> m_argumentStorage;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
template<typename ExpressionParser>
std::unique_ptr<Derived> Binary<Derived>::parse(Context &context,
	ExpressionContext &expressionContext, ExpressionParser parseExpression)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.testAndSkip<std::string>("(")
		|| !parser.testIdentifierAndSkip(Derived::Identifier))
	{
		parser.seek(position);
		return nullptr;
	}

	auto expression = std::make_unique<Derived>();

	// Assume that expression identifier (imply, exists, etc.) is already parsed
	// Parse arguments of the expression
	expression->Binary<Derived>::setArgument<0>(parseExpression(context, expressionContext));
	expression->Binary<Derived>::setArgument<1>(parseExpression(context, expressionContext));

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
template<size_t i>
void Binary<Derived>::setArgument(const Expression *expression)
{
	static_assert(i <= 2, "Index out of range");

	m_argumentStorage[i] = nullptr;
	m_arguments[i] = expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
template<size_t i>
void Binary<Derived>::setArgument(ExpressionPointer &&expression)
{
	static_assert(i <= 2, "Index out of range");

	m_argumentStorage[i] = std::move(expression);
	m_arguments[i] = m_argumentStorage[i].get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
const std::array<const Expression *, 2> &Binary<Derived>::arguments() const
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
