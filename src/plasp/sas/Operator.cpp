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
		operator_.m_preconditions.emplace_back(Condition::fromSAS(istream, variables));

	const auto numberOfEffects = utils::parse<size_t>(istream);
	operator_.m_effects.reserve(numberOfEffects);

	for (size_t j = 0; j < numberOfEffects; j++)
		operator_.m_effects.emplace_back(Effect::fromSAS(istream, variables, operator_.m_preconditions));

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
