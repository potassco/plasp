#ifndef __PLASP__PDDL__EXPRESSIONS__QUANTIFIED_H
#define __PLASP__PDDL__EXPRESSIONS__QUANTIFIED_H

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/expressions/Variable.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Quantified
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Quantified: public Expression
{
	public:
		void setArgument(ExpressionPointer argument);
		ExpressionPointer argument() const;

		Variables &variables();
		const Variables &variables() const;

	protected:
		Variables m_variables;
		ExpressionPointer m_argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
class QuantifiedCRTP: public Quantified
{
	public:
		template<typename ExpressionParser>
		static boost::intrusive_ptr<Derived> parse(Context &context,
			ExpressionContext &expressionContext, ExpressionParser parseExpression);

	public:
		Type expressionType() const override final
		{
			return Derived::ExpressionType;
		}

		ExpressionPointer copy() override;

		ExpressionPointer reduced() override;
		ExpressionPointer negationNormalized() override;
		ExpressionPointer prenex(Expression::Type lastExpressionType) override;
		ExpressionPointer simplified() override;
		ExpressionPointer disjunctionNormalized() override;

		void print(std::ostream &ostream) const override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
template<typename ExpressionParser>
boost::intrusive_ptr<Derived> QuantifiedCRTP<Derived>::parse(Context &context,
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

	// Parse variable list
	parser.expect<std::string>("(");
	Variable::parseTypedDeclarations(context, expressionContext, expression->m_variables);
	parser.expect<std::string>(")");

	// Push newly parsed variables to the stack
	expressionContext.variables.push(&expression->m_variables);

	// Parse argument of the expression
	expression->Quantified::setArgument(parseExpression(context, expressionContext));

	// Clean up variable stack
	expressionContext.variables.pop();

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
ExpressionPointer QuantifiedCRTP<Derived>::copy()
{
	auto result = new Derived;

	result->m_argument = m_argument->copy();

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Quantified::setArgument(ExpressionPointer expression)
{
	m_argument = expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline ExpressionPointer Quantified::argument() const
{
	return m_argument;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline Variables &Quantified::variables()
{
	return m_variables;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline const Variables &Quantified::variables() const
{
	return m_variables;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer QuantifiedCRTP<Derived>::reduced()
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->reduced();

	// Child quantifiers may not move before this quantifier, the order matters
	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer QuantifiedCRTP<Derived>::negationNormalized()
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->negationNormalized();

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer QuantifiedCRTP<Derived>::prenex(Expression::Type)
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->prenex(Derived::ExpressionType);

	// Quantifiers may not move before other quantifiers, their order matters
	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer QuantifiedCRTP<Derived>::simplified()
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->simplified();

	// Associate same-type children, such as (forall (?x) (forall (?y) (...)))
	if (m_argument->expressionType() != Derived::ExpressionType)
		return this;

	auto &quantifiedExpression = dynamic_cast<Derived &>(*m_argument);

	// Unify variables
	m_variables.insert(m_variables.end(), quantifiedExpression.variables().begin(), quantifiedExpression.variables().end());

	// Move child expression up
	m_argument = quantifiedExpression.argument();

	// TODO: introduce/handle boolean values

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer QuantifiedCRTP<Derived>::disjunctionNormalized()
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->disjunctionNormalized();

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline void QuantifiedCRTP<Derived>::print(std::ostream &ostream) const
{
	ostream << "(" << Derived::Identifier << " (";

	for (size_t i = 0; i < m_variables.size(); i++)
	{
		if (i > 0)
			ostream << " ";

		m_variables[i]->print(ostream);
	}

	ostream << ") ";

	m_argument->print(ostream);

	ostream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
