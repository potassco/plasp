#include <plasp/sas/Goal.h>

#include <iostream>

#include <plasp/utils/Parsing.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Goal
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Goal Goal::fromSAS(std::istream &istream, const Variables &variables)
{
	Goal goal;

	utils::parseExpected<std::string>(istream, "begin_goal");

	const auto numberOfGoalFacts = utils::parse<size_t>(istream);
	goal.m_facts.reserve(numberOfGoalFacts);

	for (size_t i = 0; i < numberOfGoalFacts; i++)
		goal.m_facts.emplace_back(AssignedVariable::fromSAS(istream, variables));

	utils::parseExpected<std::string>(istream, "end_goal");

	return goal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Goal::Facts &Goal::facts() const
{
	return m_facts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
