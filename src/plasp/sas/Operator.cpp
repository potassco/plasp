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

	operator_.m_predicate = Predicate::fromSAS(istream);

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
