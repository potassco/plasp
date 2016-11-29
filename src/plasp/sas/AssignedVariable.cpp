#include <plasp/sas/AssignedVariable.h>

#include <iostream>

#include <boost/assert.hpp>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AssignedVariable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

AssignedVariable::AssignedVariable()
:	m_variable{nullptr},
	m_value{nullptr}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AssignedVariable::AssignedVariable(const Variable &variable, const Value &value)
:	m_variable{&variable},
	m_value{&value}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AssignedVariable AssignedVariable::fromSAS(input::Parser<> &parser, const Variables &variables)
{
	AssignedVariable assignedVariable;

	assignedVariable.m_variable = &Variable::referenceFromSAS(parser, variables);
	assignedVariable.m_value = &Value::referenceFromSAS(parser, *assignedVariable.m_variable);

	return assignedVariable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AssignedVariable AssignedVariable::fromSAS(input::Parser<> &parser, const Variable &variable)
{
	AssignedVariable assignedVariable;

	assignedVariable.m_variable = &variable;
	assignedVariable.m_value = &Value::referenceFromSAS(parser, *assignedVariable.m_variable);

	return assignedVariable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable &AssignedVariable::variable() const
{
	BOOST_ASSERT(m_variable != nullptr);

	return *m_variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value &AssignedVariable::value() const
{
	BOOST_ASSERT(m_value != nullptr);

	return *m_value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
