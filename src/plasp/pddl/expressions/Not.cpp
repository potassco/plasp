#include <plasp/pddl/expressions/Not.h>

#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Exists.h>
#include <plasp/pddl/expressions/ForAll.h>
#include <plasp/pddl/expressions/Or.h>

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

Not::Not()
:	m_argument{nullptr}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Not::setArgument(ExpressionPointer argument)
{
	m_argument = argument;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Not::argument() const
{
	return m_argument;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Not::reduced()
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->reduced();

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Not::negationNormalized()
{
	BOOST_ASSERT(m_argument);

	// Remove immediate double negations
	if (m_argument->expressionType() == Expression::Type::Not)
	{
		auto &argument = dynamic_cast<Not &>(*m_argument);

		return argument.m_argument->negationNormalized();
	}

	// Normalize argument
	m_argument = m_argument->negationNormalized();

	// Remove double negations occurring after normalizing the argument
	if (m_argument->expressionType() == Expression::Type::Not)
	{
		auto &argument = dynamic_cast<Not &>(*m_argument);

		return argument.m_argument;
	}

	// De Morgan for negative conjunctions
	if (m_argument->expressionType() == Expression::Type::And)
	{
		auto &andExpression = dynamic_cast<And &>(*m_argument);
		auto orExpression = OrPointer(new Or);

		orExpression->arguments().reserve(andExpression.arguments().size());

		for (size_t i = 0; i < andExpression.arguments().size(); i++)
			orExpression->addArgument(andExpression.arguments()[i]->negated());

		return orExpression->negationNormalized();
	}

	// De Morgan for negative disjunctions
	if (m_argument->expressionType() == Expression::Type::Or)
	{
		auto &orExpression = dynamic_cast<Or &>(*m_argument);
		auto andExpression = AndPointer(new And);

		andExpression->arguments().reserve(orExpression.arguments().size());

		for (size_t i = 0; i < orExpression.arguments().size(); i++)
			andExpression->addArgument(orExpression.arguments()[i]->negated());

		return andExpression->negationNormalized();
	}

	// De Morgen for existential quantifiers
	if (m_argument->expressionType() == Expression::Type::Exists)
	{
		auto &existsExpression = dynamic_cast<Exists &>(*m_argument);
		auto forAllExpression = ForAllPointer(new ForAll);

		forAllExpression->setArgument(existsExpression.argument()->negated());

		return forAllExpression->negationNormalized();
	}

	// De Morgen for universal quantifiers
	if (m_argument->expressionType() == Expression::Type::ForAll)
	{
		auto &forAllExpression = dynamic_cast<ForAll &>(*m_argument);
		auto existsExpression = ExistsPointer(new Exists);

		existsExpression->setArgument(forAllExpression.argument()->negated());

		return existsExpression->negationNormalized();
	}

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Not::print(std::ostream &ostream) const
{
	ostream << "(not ";

	m_argument->print(ostream);

	ostream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
