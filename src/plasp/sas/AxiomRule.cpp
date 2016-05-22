#include <plasp/sas/AxiomRule.h>

#include <iostream>

#include <plasp/sas/VariableTransition.h>
#include <plasp/utils/Parsing.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AxiomRule
//
////////////////////////////////////////////////////////////////////////////////////////////////////

AxiomRule::AxiomRule(AxiomRule::Conditions conditions, AxiomRule::Condition postcondition)
:	m_conditions(std::move(conditions)),
	m_postcondition(std::move(postcondition))
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AxiomRule AxiomRule::fromSAS(std::istream &istream, const Variables &variables)
{
	utils::parseExpected<std::string>(istream, "begin_rule");

	const auto numberOfConditions = utils::parse<size_t>(istream);

	Conditions conditions;
	conditions.reserve(numberOfConditions);

	for (size_t j = 0; j < numberOfConditions; j++)
		conditions.emplace_back(AssignedVariable::fromSAS(istream, variables));

	const auto variableTransition = VariableTransition::fromSAS(istream, variables);

	if (&variableTransition.valueBefore() != &Value::Any)
		conditions.emplace_back(AssignedVariable(variableTransition.variable(), variableTransition.valueBefore()));

	utils::parseExpected<std::string>(istream, "end_rule");

	const Condition postcondition(variableTransition.variable(), variableTransition.valueAfter());
	const AxiomRule axiomRule(std::move(conditions), std::move(postcondition));

	return axiomRule;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const AxiomRule::Conditions &AxiomRule::conditions() const
{
	return m_conditions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const AxiomRule::Condition &AxiomRule::postcondition() const
{
	return m_postcondition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
