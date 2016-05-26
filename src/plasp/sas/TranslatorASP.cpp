#include <plasp/sas/TranslatorASP.h>

#include <plasp/sas/TranslatorException.h>
#include <plasp/utils/Parsing.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TranslatorASP
//
////////////////////////////////////////////////////////////////////////////////////////////////////

TranslatorASP::TranslatorASP(const Description &description)
:	m_description(description)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translate(std::ostream &ostream) const
{
	translateRequirements(ostream);
	translateInitialState(ostream);
	translateGoal(ostream);
	translateVariables(ostream);
	translateActions(ostream);
	translateMutexes(ostream);
	translateAxiomRules(ostream);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateRequirements(std::ostream &ostream) const
{
	ostream << "% feature requirements" << std::endl;

	if (m_description.usesActionCosts())
		ostream << "requiresFeature(actionCosts)." << std::endl;

	if (m_description.usesAxiomRules())
		ostream << "requiresFeature(axiomRules)." << std::endl;

	if (m_description.usesConditionalEffects())
		ostream << "requiresFeature(conditionalEffects)." << std::endl;

	ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateInitialState(std::ostream &ostream) const
{
	ostream << "% initial state" << std::endl;

	const auto &initialStateFacts = m_description.initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			ostream << "initialState(";
			fact.variable().printNameAsASPPredicate(ostream);
			ostream << ", ";
			fact.value().printAsASPPredicate(ostream);
			ostream << ")." << std::endl;
		});

	ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateGoal(std::ostream &ostream) const
{
	ostream << "% goal" << std::endl;

	const auto &goalFacts = m_description.goal().facts();

	std::for_each(goalFacts.cbegin(), goalFacts.cend(),
		[&](const auto &fact)
		{
			ostream << "goal(";
			fact.variable().printNameAsASPPredicate(ostream);
			ostream << ", ";
			fact.value().printAsASPPredicate(ostream);
			ostream << ")." << std::endl;
		});

	ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateVariables(std::ostream &ostream) const
{
	ostream << "% variables";

	const auto &variables = m_description.variables();

	std::for_each(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			const auto &values = variable.values();

			BOOST_ASSERT(!values.empty());

			ostream << std::endl;
			variable.printNameAsASPPredicate(ostream);
			ostream << "." << std::endl;

			std::for_each(values.cbegin(), values.cend(),
				[&](const auto &value)
				{
					ostream << "contains(";
					variable.printNameAsASPPredicate(ostream);
					ostream << ", ";
					value.printAsASPPredicate(ostream);
					ostream << ")." << std::endl;
				});
		});

	ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions(std::ostream &ostream) const
{
	ostream << "% actions";

	const auto &operators = m_description.operators();

	std::for_each(operators.cbegin(), operators.cend(),
		[&](const auto &operator_)
		{
			ostream << std::endl;
			operator_.printPredicateAsASP(ostream);
			ostream << "." << std::endl;

			const auto &preconditions = operator_.preconditions();

			std::for_each(preconditions.cbegin(), preconditions.cend(),
				[&](const auto &precondition)
				{
					ostream << "precondition(";
					operator_.printPredicateAsASP(ostream);
					ostream << ", ";
					precondition.variable().printNameAsASPPredicate(ostream);
					ostream << ", ";
					precondition.value().printAsASPPredicate(ostream);
					ostream << ")." << std::endl;
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
							ostream << "effectCondition(";
							operator_.printPredicateAsASP(ostream);
							ostream << ", effect(" << currentEffectID << "), ";
							condition.variable().printNameAsASPPredicate(ostream);
							ostream << ", ";
							condition.value().printAsASPPredicate(ostream);
							ostream << ")." << std::endl;
						});

					ostream << "postcondition(";
					operator_.printPredicateAsASP(ostream);
					ostream << ", effect(" << currentEffectID << "), ";
					effect.postcondition().variable().printNameAsASPPredicate(ostream);
					ostream << ", ";
					effect.postcondition().value().printAsASPPredicate(ostream);
					ostream << ")." << std::endl;

					currentEffectID++;
				});

			ostream << "costs(";
			operator_.printPredicateAsASP(ostream);
			ostream << ", " << operator_.costs() << ")." << std::endl;
		});

	ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateMutexes(std::ostream &ostream) const
{
	ostream << "% mutex groups";

	const auto &mutexGroups = m_description.mutexGroups();

	size_t currentMutexGroupID = 0;

	std::for_each(mutexGroups.cbegin(), mutexGroups.cend(),
		[&](const auto &mutexGroup)
		{
			const auto mutexGroupID = std::to_string(currentMutexGroupID);
			currentMutexGroupID++;

			ostream << std::endl << "mutexGroup(" << mutexGroupID << ")." << std::endl;

			const auto &facts = mutexGroup.facts();

			std::for_each(facts.cbegin(), facts.cend(),
				[&](const auto &fact)
				{
					ostream << "contains(mutexGroup(" << mutexGroupID << "), ";
					fact.variable().printNameAsASPPredicate(ostream);
					ostream << ", ";
					fact.value().printAsASPPredicate(ostream);
					ostream << ")." << std::endl;
				});
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateAxiomRules(std::ostream &ostream) const
{
	if (!m_description.usesActionCosts())
		return;

	ostream << std::endl;
	ostream << "% axiom rules";

	const auto &axiomRules = m_description.axiomRules();

	size_t currentAxiomRuleID = 0;

	std::for_each(axiomRules.cbegin(), axiomRules.cend(),
		[&](const auto &axiomRule)
		{
			const auto axiomRuleID = std::to_string(currentAxiomRuleID);
			currentAxiomRuleID++;

			ostream << std::endl << "axiomRule(" << axiomRuleID << ")." << std::endl;

			const auto &conditions = axiomRule.conditions();

			std::for_each(conditions.cbegin(), conditions.cend(),
				[&](const auto &condition)
				{
					ostream << "condition(axiomRule(" << axiomRuleID << "), ";
					condition.variable().printNameAsASPPredicate(ostream);
					ostream << ", ";
					condition.value().printAsASPPredicate(ostream);
					ostream << ")." << std::endl;
				});

			const auto &postcondition = axiomRule.postcondition();

			ostream << "postcondition(axiomRule(axiomRule" << axiomRuleID << "), ";
			postcondition.variable().printNameAsASPPredicate(ostream);
			ostream << ", ";
			postcondition.value().printAsASPPredicate(ostream);
			ostream << ")." << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
