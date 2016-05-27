#include <plasp/sas/AxiomRule.h>

#include <iostream>

#include <plasp/sas/VariableTransition.h>

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

AxiomRule AxiomRule::fromSAS(utils::Parser &parser, const Variables &variables)
{
	parser.expect<std::string>("begin_rule");

	const auto numberOfConditions = parser.parse<size_t>();

	Conditions conditions;
	conditions.reserve(numberOfConditions);

	for (size_t j = 0; j < numberOfConditions; j++)
		conditions.emplace_back(Condition::fromSAS(parser, variables));

	const auto variableTransition = VariableTransition::fromSAS(parser, variables);

	if (&variableTransition.valueBefore() != &Value::Any)
		conditions.emplace_back(Condition(variableTransition.variable(), variableTransition.valueBefore()));

	parser.expect<std::string>("end_rule");

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
