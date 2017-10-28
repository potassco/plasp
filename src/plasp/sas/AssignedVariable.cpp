#include <plasp/sas/AssignedVariable.h>

#include <cassert>
#include <iostream>

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

AssignedVariable AssignedVariable::fromSAS(tokenize::Tokenizer<> &tokenizer, const Variables &variables)
{
	AssignedVariable assignedVariable;

	assignedVariable.m_variable = &Variable::referenceFromSAS(tokenizer, variables);
	assignedVariable.m_value = &Value::referenceFromSAS(tokenizer, *assignedVariable.m_variable);

	return assignedVariable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AssignedVariable AssignedVariable::fromSAS(tokenize::Tokenizer<> &tokenizer, const Variable &variable)
{
	AssignedVariable assignedVariable;

	assignedVariable.m_variable = &variable;
	assignedVariable.m_value = &Value::referenceFromSAS(tokenizer, *assignedVariable.m_variable);

	return assignedVariable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable &AssignedVariable::variable() const
{
	assert(m_variable != nullptr);

	return *m_variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value &AssignedVariable::value() const
{
	assert(m_value != nullptr);

	return *m_value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
