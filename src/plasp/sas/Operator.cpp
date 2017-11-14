#include <plasp/sas/Operator.h>

#include <iostream>
#include <limits>

#include <colorlog/Formatting.h>

#include <plasp/sas/VariableTransition.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Operator
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Operator Operator::fromSAS(tokenize::Tokenizer<> &tokenizer, const Variables &variables)
{
	Operator operator_;

	tokenizer.expect<std::string>("begin_operator");

	operator_.m_predicate = Predicate::fromSAS(tokenizer);

	const auto numberOfPrevailConditions = tokenizer.get<size_t>();
	operator_.m_preconditions.reserve(numberOfPrevailConditions);

	for (size_t j = 0; j < numberOfPrevailConditions; j++)
		operator_.m_preconditions.emplace_back(Condition::fromSAS(tokenizer, variables));

	const auto numberOfEffects = tokenizer.get<size_t>();
	operator_.m_effects.reserve(numberOfEffects);

	for (size_t j = 0; j < numberOfEffects; j++)
		operator_.m_effects.emplace_back(Effect::fromSAS(tokenizer, variables, operator_.m_preconditions));

	operator_.m_costs = tokenizer.get<size_t>();

	tokenizer.expect<std::string>("end_operator");

	return operator_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Operator::printPredicateAsASP(colorlog::ColorStream &stream) const
{
	stream << colorlog::Keyword("action") << "(";
	m_predicate.printAsASP(stream);
	stream << ")";
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
