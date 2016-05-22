#include <plasp/sas/InitialState.h>

#include <iostream>

#include <plasp/utils/Parsing.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InitialState
//
////////////////////////////////////////////////////////////////////////////////////////////////////

InitialState InitialState::fromSAS(std::istream &istream, const std::vector<Variable> &variables)
{
	InitialState initialState;

	utils::parseExpected<std::string>(istream, "begin_state");

	initialState.m_facts.reserve(variables.size());

	for (size_t i = 0; i < variables.size(); i++)
		initialState.m_facts.emplace_back(AssignedVariable::fromSAS(istream, variables[i]));

	utils::parseExpected<std::string>(istream, "end_state");

	return initialState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const InitialState::Facts &InitialState::facts() const
{
	return m_facts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
