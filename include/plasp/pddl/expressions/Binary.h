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
		static boost::intrusive_ptr<Derived> parse(Context &context,
			ExpressionContext &expressionContext, ExpressionParser parseExpression);

	public:
		void setArgument(size_t i, ExpressionPointer argument);
		const std::array<ExpressionPointer, 2> &arguments() const;

		ExpressionPointer normalized() override;

	protected:
		std::array<ExpressionPointer, 2> m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
template<typename ExpressionParser>
boost::intrusive_ptr<Derived> Binary<Derived>::parse(Context &context,
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

	auto expression = boost::intrusive_ptr<Derived>(new Derived);

	// Assume that expression identifier (imply, exists, etc.) is already parsed
	// Parse arguments of the expression
	expression->Binary<Derived>::setArgument(0, parseExpression(context, expressionContext));
	expression->Binary<Derived>::setArgument(1, parseExpression(context, expressionContext));

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
void Binary<Derived>::setArgument(size_t i, ExpressionPointer expression)
{
	BOOST_ASSERT_MSG(i <= m_arguments.size(), "Index out of range");

	m_arguments[i] = expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
const std::array<ExpressionPointer, 2> &Binary<Derived>::arguments() const
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer Binary<Derived>::normalized()
{
	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		BOOST_ASSERT(m_arguments[i]);

		m_arguments[i] = m_arguments[i]->normalized();
	}

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
