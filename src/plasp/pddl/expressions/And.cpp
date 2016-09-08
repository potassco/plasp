#include <plasp/pddl/expressions/And.h>

#include <algorithm>
#include <iostream>

#include <plasp/pddl/expressions/Or.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// And
//
////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string And::Identifier = "and";

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer And::disjunctionNormalized()
{
	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		BOOST_ASSERT(m_arguments[i]);

		m_arguments[i] = m_arguments[i]->disjunctionNormalized();
	}

	const auto match = std::find_if(m_arguments.begin(), m_arguments.end(),
		[](const auto &argument)
		{
			return argument->expressionType() == Expression::Type::Or;
		});

	if (match == m_arguments.end())
		return this;

	auto orExpression = OrPointer(dynamic_cast<expressions::Or *>(match->get()));
	const size_t orExpressionIndex = match - m_arguments.begin();

	// Apply the distributive law
	// Copy this and expression for each argument of the or expression
	for (size_t i = 0; i < orExpression->arguments().size(); i++)
	{
		auto newAndExpression = new expressions::And;
		newAndExpression->arguments().resize(m_arguments.size());

		for (size_t j = 0; j < m_arguments.size(); j++)
		{
			if (j == orExpressionIndex)
				newAndExpression->arguments()[j] = orExpression->arguments()[i]->copy();
			else
				newAndExpression->arguments()[j] = m_arguments[j]->copy();
		}

		// Replace the respective argument with the new, recursively normalized and expression
		orExpression->arguments()[i] = newAndExpression->disjunctionNormalized();
	}

	return orExpression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
