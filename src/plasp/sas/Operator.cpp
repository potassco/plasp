#include <plasp/sas/Operator.h>

#include <iostream>
#include <limits>

#include <plasp/sas/VariableTransition.h>
#include <plasp/utils/Formatting.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Operator
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Operator Operator::fromSAS(utils::Parser &parser, const Variables &variables)
{
	Operator operator_;

	parser.expect<std::string>("begin_operator");

	operator_.m_predicate = Predicate::fromSAS(parser);

	const auto numberOfPrevailConditions = parser.parse<size_t>();
	operator_.m_preconditions.reserve(numberOfPrevailConditions);

	for (size_t j = 0; j < numberOfPrevailConditions; j++)
		operator_.m_preconditions.emplace_back(Condition::fromSAS(parser, variables));

	const auto numberOfEffects = parser.parse<size_t>();
	operator_.m_effects.reserve(numberOfEffects);

	for (size_t j = 0; j < numberOfEffects; j++)
		operator_.m_effects.emplace_back(Effect::fromSAS(parser, variables, operator_.m_preconditions));

	operator_.m_costs = parser.parse<size_t>();

	parser.expect<std::string>("end_operator");

	return operator_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Operator::printPredicateAsASP(utils::LogStream &outputStream) const
{
	outputStream << utils::Keyword("action") << "(";
	m_predicate.printAsASP(outputStream);
	outputStream << ")";
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
