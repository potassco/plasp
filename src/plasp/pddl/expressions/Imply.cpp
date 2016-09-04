#include <plasp/pddl/expressions/Imply.h>

#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Or.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Imply
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: make static character string literal
const std::string Imply::Identifier = "imply";

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Imply::normalized()
{
	BOOST_ASSERT(m_arguments[0]);
	BOOST_ASSERT(m_arguments[1]);

	m_arguments[0] = m_arguments[0]->normalized();
	m_arguments[1] = m_arguments[1]->normalized();

	auto notArgument0 = NotPointer(new Not);
	notArgument0->setArgument(m_arguments[0]);

	auto orExpression = OrPointer(new Or);
	orExpression->addArgument(notArgument0);
	orExpression->addArgument(m_arguments[1]);

	auto normalizedOrExpression = orExpression->normalized();

	if (normalizedOrExpression)
		return normalizedOrExpression;

	return std::move(orExpression);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
