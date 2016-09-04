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

ExpressionPointer Imply::normalize()
{
	BOOST_ASSERT(m_argumentStorage[0]);
	BOOST_ASSERT(m_argumentStorage[1]);

	auto notArgument0 = NotPointer(new Not);
	notArgument0->setArgument(std::move(m_argumentStorage[0]));

	auto orExpression = OrPointer(new Or);
	orExpression->addArgument(std::move(notArgument0));
	orExpression->addArgument(std::move(m_argumentStorage[1]));

	auto normalizedOrExpression = orExpression->normalize();

	if (normalizedOrExpression)
		return normalizedOrExpression;

	return std::move(orExpression);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
