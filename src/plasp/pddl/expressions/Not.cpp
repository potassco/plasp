#include <plasp/pddl/expressions/Not.h>

#include <plasp/output/TranslatorException.h>
#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/DerivedPredicate.h>
#include <plasp/pddl/expressions/Exists.h>
#include <plasp/pddl/expressions/ForAll.h>
#include <plasp/pddl/expressions/Or.h>
#include <plasp/pddl/expressions/Predicate.h>

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

ExpressionPointer Not::copy()
{
	auto result = new Not;

	result->m_argument = m_argument->copy();

	return result;
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

ExpressionPointer Not::existentiallyQuantified()
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->existentiallyQuantified();

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Not::simplified()
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->simplified();

	if (!m_argument->is<Not>())
		return this;

	// Remove double negations
	const auto &notExpression = m_argument->as<Not>();

	return notExpression.argument();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Not::collectParameters(std::set<VariablePointer> &parameters)
{
	m_argument->collectParameters(parameters);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Not::decomposed(DerivedPredicates &derivedPredicates)
{
	m_argument = m_argument->decomposed(derivedPredicates);

	// Predicates and derived predicates can be directly negated
	if (m_argument->is<Predicate>() || m_argument->is<DerivedPredicate>())
		return this;

	derivedPredicates.emplace_back(new DerivedPredicate());
	auto &derivedPredicate = derivedPredicates.back();

	// Move this expression’s arguments to the derived predicate
	derivedPredicate->setPreconditions({{this}});

	return derivedPredicate;
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
