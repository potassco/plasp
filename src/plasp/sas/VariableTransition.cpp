#include <plasp/sas/VariableTransition.h>

#include <iostream>

#include <boost/assert.hpp>

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

VariableTransition VariableTransition::fromSAS(parsebase::Parser<> &parser, const Variables &variables)
{
	VariableTransition variableTransition;

	variableTransition.m_variable = &Variable::referenceFromSAS(parser, variables);
	variableTransition.m_valueBefore = &Value::referenceFromSAS(parser, *variableTransition.m_variable);
	variableTransition.m_valueAfter = &Value::referenceFromSAS(parser, *variableTransition.m_variable);

	return variableTransition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable &VariableTransition::variable() const
{
	BOOST_ASSERT(m_variable != nullptr);

	return *m_variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value &VariableTransition::valueBefore() const
{
	BOOST_ASSERT(m_valueBefore != nullptr);

	return *m_valueBefore;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value &VariableTransition::valueAfter() const
{
	BOOST_ASSERT(m_valueAfter != nullptr);

	return *m_valueAfter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
