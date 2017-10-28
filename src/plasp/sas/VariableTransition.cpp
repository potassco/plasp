#include <plasp/sas/VariableTransition.h>

#include <cassert>
#include <iostream>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VariableTransition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

VariableTransition::VariableTransition()
:	m_variable{nullptr},
	m_valueBefore{nullptr},
	m_valueAfter{nullptr}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VariableTransition VariableTransition::fromSAS(tokenize::Tokenizer<> &tokenizer, const Variables &variables)
{
	VariableTransition variableTransition;

	variableTransition.m_variable = &Variable::referenceFromSAS(tokenizer, variables);
	variableTransition.m_valueBefore = &Value::referenceFromSAS(tokenizer, *variableTransition.m_variable);
	variableTransition.m_valueAfter = &Value::referenceFromSAS(tokenizer, *variableTransition.m_variable);

	return variableTransition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable &VariableTransition::variable() const
{
	assert(m_variable != nullptr);

	return *m_variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value &VariableTransition::valueBefore() const
{
	assert(m_valueBefore != nullptr);

	return *m_valueBefore;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value &VariableTransition::valueAfter() const
{
	assert(m_valueAfter != nullptr);

	return *m_valueAfter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
