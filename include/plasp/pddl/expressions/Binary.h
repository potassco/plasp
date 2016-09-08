#ifndef __PLASP__PDDL__EXPRESSIONS__BINARY_H
#define __PLASP__PDDL__EXPRESSIONS__BINARY_H

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>

#include <plasp/pddl/expressions/Exists.h>
#include <plasp/pddl/expressions/ForAll.h>

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
		ExpressionPointer copy() override;

		void setArgument(size_t i, ExpressionPointer argument);
		const std::array<ExpressionPointer, 2> &arguments() const;

		ExpressionPointer reduced() override;
		ExpressionPointer negationNormalized() override;
		ExpressionPointer prenex(Expression::Type lastExpressionType) override;
		ExpressionPointer simplified() override;
		ExpressionPointer disjunctionNormalized() override;

		void print(std::ostream &ostream) const override;

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

	// Parse arguments of the expression
	expression->Binary<Derived>::setArgument(0, parseExpression(context, expressionContext));
	expression->Binary<Derived>::setArgument(1, parseExpression(context, expressionContext));

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
ExpressionPointer Binary<Derived>::copy()
{
	auto result = new Derived;

	for (size_t i = 0; i < m_arguments.size(); i++)
		result->m_arguments[i] = m_arguments[i]->copy();

	return result;
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
inline ExpressionPointer Binary<Derived>::reduced()
{
	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		BOOST_ASSERT(m_arguments[i]);

		m_arguments[i] = m_arguments[i]->reduced();
	}

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer Binary<Derived>::negationNormalized()
{
	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		BOOST_ASSERT(m_arguments[i]);

		m_arguments[i] = m_arguments[i]->negationNormalized();
	}

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer Binary<Derived>::prenex(Expression::Type)
{
	// TODO: implement by refactoring binary expressions
	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer Binary<Derived>::simplified()
{
	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		BOOST_ASSERT(m_arguments[i]);

		m_arguments[i] = m_arguments[i]->simplified();
	}

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer Binary<Derived>::disjunctionNormalized()
{
	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		BOOST_ASSERT(m_arguments[i]);

		m_arguments[i] = m_arguments[i]->disjunctionNormalized();
	}

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline void Binary<Derived>::print(std::ostream &ostream) const
{
	ostream << "(" << Derived::Identifier;

	std::for_each(m_arguments.begin(), m_arguments.end(),
		[&](auto &argument)
		{
			ostream << " ";
			argument->print(ostream);
		});

	ostream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
