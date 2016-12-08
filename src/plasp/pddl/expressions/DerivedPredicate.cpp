#include <plasp/pddl/expressions/DerivedPredicate.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/ExpressionContext.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DerivedPredicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

DerivedPredicate::DerivedPredicate(size_t id)
:	m_id{id}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

size_t DerivedPredicate::id() const
{
	return m_id;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DerivedPredicate::setPreconditions(std::vector<Expressions> &&preconditions)
{
	m_preconditions = std::move(preconditions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<Expressions> &DerivedPredicate::preconditions() const
{
	return m_preconditions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DerivedPredicate::collectParameters()
{
	for (const auto &conjunction : m_preconditions)
		for (const auto &precondition : conjunction)
			precondition->collectParameters(m_parameters);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::set<VariablePointer> &DerivedPredicate::parameters() const
{
	return m_parameters;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DerivedPredicate::collectParameters(std::set<VariablePointer> &parameters)
{
	for (auto &conjunction : m_preconditions)
		for (auto &precondition : conjunction)
		{
			BOOST_ASSERT(precondition.get() != this);
			precondition->collectParameters(m_parameters);
		}

	// Copy in order not to interfere with potentially bound variables in parent expressions
	parameters = m_parameters;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DerivedPredicate::print(std::ostream &ostream) const
{
	ostream << "(:derived " << m_id << " ";

	BOOST_ASSERT(m_preconditions.size() > 0);

	const auto printConjunction =
		[&ostream](const auto &conjunction)
		{
			if (conjunction.size() == 0)
			{
				conjunction.front()->print(ostream);
				return;
			}

			ostream << "(and";

			for (const auto &precondition : conjunction)
			{
				ostream << " ";
				precondition->print(ostream);
			}

			ostream << ")";
		};

	if (m_preconditions.size() == 1)
	{
		const auto &conjunction = m_preconditions.front();

		BOOST_ASSERT(conjunction.size() > 0);

		printConjunction(conjunction);

		ostream << ")";

		return;
	}

	ostream << " (or";

	for (const auto conjunction : m_preconditions)
	{
		ostream << " ";

		printConjunction(conjunction);
	}

	ostream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
