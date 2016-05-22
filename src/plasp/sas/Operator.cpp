#include <plasp/sas/Operator.h>

#include <iostream>
#include <limits>

#include <plasp/sas/VariableTransition.h>
#include <plasp/utils/Parsing.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Operator
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Operator Operator::fromSAS(std::istream &istream, const Variables &variables)
{
	Operator operator_;

	utils::parseExpected<std::string>(istream, "begin_operator");

	try
	{
		istream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// TODO: Inefficient, reimplement in one pass
		std::string line;
		std::getline(istream, line);

		std::stringstream lineStream(line);

		operator_.m_predicate.name = utils::parse<std::string>(lineStream);

		while (lineStream.peek() == ' ')
			lineStream.ignore(1);

		for (std::string argument; std::getline(lineStream, argument, ' ');)
			operator_.m_predicate.arguments.push_back(std::move(argument));
	}
	catch (const std::exception &e)
	{
		throw utils::ParserException("Could not parse operator predicate");
	}

	const auto numberOfPrevailConditions = utils::parse<size_t>(istream);
	operator_.m_preconditions.reserve(numberOfPrevailConditions);

	for (size_t j = 0; j < numberOfPrevailConditions; j++)
		operator_.m_preconditions.emplace_back(AssignedVariable::fromSAS(istream, variables));

	const auto numberOfEffects = utils::parse<size_t>(istream);
	operator_.m_effects.reserve(numberOfEffects);

	for (size_t j = 0; j < numberOfEffects; j++)
	{
		Effect::Conditions conditions;

		const auto numberOfEffectConditions = utils::parse<size_t>(istream);
		conditions.reserve(numberOfEffectConditions);

		for (size_t k = 0; k < numberOfEffectConditions; k++)
			conditions.emplace_back(AssignedVariable::fromSAS(istream, variables));

		const auto variableTransition = VariableTransition::fromSAS(istream, variables);

		if (&variableTransition.valueBefore() != &Value::Any)
			operator_.m_preconditions.emplace_back(AssignedVariable(variableTransition.variable(), variableTransition.valueBefore()));

		const Effect::Condition postcondition = {variableTransition.variable(), variableTransition.valueAfter()};
		const Effect effect = {std::move(conditions), std::move(postcondition)};
		operator_.m_effects.push_back(std::move(effect));
	}

	operator_.m_costs = utils::parse<size_t>(istream);

	utils::parseExpected<std::string>(istream, "end_operator");

	return operator_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Predicate &Operator::predicate() const
{
	return m_predicate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Operator::Conditions &Operator::preconditions() const
{
	return m_preconditions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Effects &Operator::effects() const
{
	return m_effects;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

size_t Operator::costs() const
{
	return m_costs;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
