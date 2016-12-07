#include <plasp/pddl/expressions/ForAll.h>

#include <algorithm>
#include <iostream>

#include <plasp/pddl/expressions/Exists.h>
#include <plasp/pddl/expressions/Not.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ForAll
//
////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string ForAll::Identifier = "forall";

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer ForAll::existentiallyQuantified()
{
	auto existsExpression = ExistsPointer(new Exists());
	auto notExpression = NotPointer(new Not());

	notExpression->setArgument(m_argument->existentiallyQuantified());
	existsExpression->setArgument(notExpression);
	existsExpression->variables() = std::move(m_variables);

	return existsExpression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
