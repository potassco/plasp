#include <plasp/sas/TranslatorASP.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TranslatorASP
//
////////////////////////////////////////////////////////////////////////////////////////////////////

TranslatorASP::TranslatorASP(const Description &description, std::ostream &ostream)
:	m_description(description),
	m_ostream(ostream)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translate() const
{
	translateRequirements();
	translateInitialState();
	translateGoal();
	translateVariables();
	translateActions();
	translateMutexes();
	translateAxiomRules();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateRequirements() const
{
	m_ostream << "% feature requirements" << std::endl;

	if (m_description.usesActionCosts())
		m_ostream << "requiresFeature(actionCosts)." << std::endl;

	if (m_description.usesAxiomRules())
		m_ostream << "requiresFeature(axiomRules)." << std::endl;

	if (m_description.usesConditionalEffects())
		m_ostream << "requiresFeature(conditionalEffects)." << std::endl;

	m_ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateInitialState() const
{
	m_ostream << "% initial state" << std::endl;

	const auto &initialStateFacts = m_description.initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			m_ostream << "initialState(";
			fact.variable().printNameAsASPPredicate(m_ostream);
			m_ostream << ", ";
			fact.value().printAsASPPredicate(m_ostream);
			m_ostream << ")." << std::endl;
		});

	m_ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateGoal() const
{
	m_ostream << "% goal" << std::endl;

	const auto &goalFacts = m_description.goal().facts();

	std::for_each(goalFacts.cbegin(), goalFacts.cend(),
		[&](const auto &fact)
		{
			m_ostream << "goal(";
			fact.variable().printNameAsASPPredicate(m_ostream);
			m_ostream << ", ";
			fact.value().printAsASPPredicate(m_ostream);
			m_ostream << ")." << std::endl;
		});

	m_ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateVariables() const
{
	m_ostream << "% variables";

	const auto &variables = m_description.variables();

	std::for_each(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			const auto &values = variable.values();

			BOOST_ASSERT(!values.empty());

			m_ostream << std::endl;
			variable.printNameAsASPPredicate(m_ostream);
			m_ostream << "." << std::endl;

			std::for_each(values.cbegin(), values.cend(),
				[&](const auto &value)
				{
					m_ostream << "contains(";
					variable.printNameAsASPPredicate(m_ostream);
					m_ostream << ", ";
					value.printAsASPPredicate(m_ostream);
					m_ostream << ")." << std::endl;
				});
		});

	m_ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions() const
{
	m_ostream << "% actions";

	const auto &operators = m_description.operators();

	std::for_each(operators.cbegin(), operators.cend(),
		[&](const auto &operator_)
		{
			m_ostream << std::endl;
			operator_.printPredicateAsASP(m_ostream);
			m_ostream << "." << std::endl;

			const auto &preconditions = operator_.preconditions();

			std::for_each(preconditions.cbegin(), preconditions.cend(),
				[&](const auto &precondition)
				{
					m_ostream << "precondition(";
					operator_.printPredicateAsASP(m_ostream);
					m_ostream << ", ";
					precondition.variable().printNameAsASPPredicate(m_ostream);
					m_ostream << ", ";
					precondition.value().printAsASPPredicate(m_ostream);
					m_ostream << ")." << std::endl;
				});

			const auto &effects = operator_.effects();

			size_t currentEffectID = 0;

			std::for_each(effects.cbegin(), effects.cend(),
				[&](const auto &effect)
				{
					const auto &conditions = effect.conditions();

					std::for_each(conditions.cbegin(), conditions.cend(),
						[&](const auto &condition)
						{
							m_ostream << "effectCondition(";
							operator_.printPredicateAsASP(m_ostream);
							m_ostream << ", effect(" << currentEffectID << "), ";
							condition.variable().printNameAsASPPredicate(m_ostream);
							m_ostream << ", ";
							condition.value().printAsASPPredicate(m_ostream);
							m_ostream << ")." << std::endl;
						});

					m_ostream << "postcondition(";
					operator_.printPredicateAsASP(m_ostream);
					m_ostream << ", effect(" << currentEffectID << "), ";
					effect.postcondition().variable().printNameAsASPPredicate(m_ostream);
					m_ostream << ", ";
					effect.postcondition().value().printAsASPPredicate(m_ostream);
					m_ostream << ")." << std::endl;

					currentEffectID++;
				});

			m_ostream << "costs(";
			operator_.printPredicateAsASP(m_ostream);
			m_ostream << ", " << operator_.costs() << ")." << std::endl;
		});

	m_ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateMutexes() const
{
	m_ostream << "% mutex groups";

	const auto &mutexGroups = m_description.mutexGroups();

	size_t currentMutexGroupID = 0;

	std::for_each(mutexGroups.cbegin(), mutexGroups.cend(),
		[&](const auto &mutexGroup)
		{
			const auto mutexGroupID = std::to_string(currentMutexGroupID);
			currentMutexGroupID++;

			m_ostream << std::endl << "mutexGroup(" << mutexGroupID << ")." << std::endl;

			const auto &facts = mutexGroup.facts();

			std::for_each(facts.cbegin(), facts.cend(),
				[&](const auto &fact)
				{
					m_ostream << "contains(mutexGroup(" << mutexGroupID << "), ";
					fact.variable().printNameAsASPPredicate(m_ostream);
					m_ostream << ", ";
					fact.value().printAsASPPredicate(m_ostream);
					m_ostream << ")." << std::endl;
				});
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateAxiomRules() const
{
	if (!m_description.usesActionCosts())
		return;

	m_ostream << std::endl;
	m_ostream << "% axiom rules";

	const auto &axiomRules = m_description.axiomRules();

	size_t currentAxiomRuleID = 0;

	std::for_each(axiomRules.cbegin(), axiomRules.cend(),
		[&](const auto &axiomRule)
		{
			const auto axiomRuleID = std::to_string(currentAxiomRuleID);
			currentAxiomRuleID++;

			m_ostream << std::endl << "axiomRule(" << axiomRuleID << ")." << std::endl;

			const auto &conditions = axiomRule.conditions();

			std::for_each(conditions.cbegin(), conditions.cend(),
				[&](const auto &condition)
				{
					m_ostream << "condition(axiomRule(" << axiomRuleID << "), ";
					condition.variable().printNameAsASPPredicate(m_ostream);
					m_ostream << ", ";
					condition.value().printAsASPPredicate(m_ostream);
					m_ostream << ")." << std::endl;
				});

			const auto &postcondition = axiomRule.postcondition();

			m_ostream << "postcondition(axiomRule(axiomRule" << axiomRuleID << "), ";
			postcondition.variable().printNameAsASPPredicate(m_ostream);
			m_ostream << ", ";
			postcondition.value().printAsASPPredicate(m_ostream);
			m_ostream << ")." << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
