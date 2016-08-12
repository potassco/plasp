#include <plasp/sas/TranslatorASP.h>

#include <plasp/utils/Formatting.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TranslatorASP
//
////////////////////////////////////////////////////////////////////////////////////////////////////

TranslatorASP::TranslatorASP(const Description &description, utils::LogStream &ostream)
:	m_description(description),
	m_outputStream(ostream)
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
	m_outputStream << utils::Heading2("feature requirements") << std::endl;

	if (m_description.usesActionCosts())
		m_outputStream << utils::Keyword("requiresFeature") << "(actionCosts)." << std::endl;

	if (m_description.usesAxiomRules())
		m_outputStream << utils::Keyword("requiresFeature") << "(axiomRules)." << std::endl;

	if (m_description.usesConditionalEffects())
		m_outputStream << utils::Keyword("requiresFeature") << "(conditionalEffects)." << std::endl;

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateInitialState() const
{
	m_outputStream << utils::Heading2("initial state") << std::endl;

	const auto &initialStateFacts = m_description.initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			m_outputStream << utils::Keyword("initialState") << "(";
			fact.variable().printNameAsASPPredicate(m_outputStream);
			m_outputStream << ", ";
			fact.value().printAsASPPredicate(m_outputStream);
			m_outputStream << ")." << std::endl;
		});

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateGoal() const
{
	m_outputStream << utils::Heading2("goal") << std::endl;

	const auto &goalFacts = m_description.goal().facts();

	std::for_each(goalFacts.cbegin(), goalFacts.cend(),
		[&](const auto &fact)
		{
			m_outputStream << utils::Keyword("goal") << "(";
			fact.variable().printNameAsASPPredicate(m_outputStream);
			m_outputStream << ", ";
			fact.value().printAsASPPredicate(m_outputStream);
			m_outputStream << ")." << std::endl;
		});

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateVariables() const
{
	m_outputStream << utils::Heading2("variables");

	const auto &variables = m_description.variables();

	std::for_each(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			const auto &values = variable.values();

			BOOST_ASSERT(!values.empty());

			m_outputStream << std::endl << utils::Keyword("variable") << "(";
			variable.printNameAsASPPredicate(m_outputStream);
			m_outputStream << ")." << std::endl;

			std::for_each(values.cbegin(), values.cend(),
				[&](const auto &value)
				{
					m_outputStream << utils::Keyword("contains") << "(";
					variable.printNameAsASPPredicate(m_outputStream);
					m_outputStream << ", ";
					value.printAsASPPredicate(m_outputStream);
					m_outputStream << ")." << std::endl;
				});
		});

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions() const
{
	m_outputStream << utils::Heading2("actions");

	const auto &operators = m_description.operators();

	std::for_each(operators.cbegin(), operators.cend(),
		[&](const auto &operator_)
		{
			m_outputStream << std::endl << utils::Keyword("action") << "(";
			operator_.printPredicateAsASP(m_outputStream);
			m_outputStream << ")." << std::endl;

			const auto &preconditions = operator_.preconditions();

			std::for_each(preconditions.cbegin(), preconditions.cend(),
				[&](const auto &precondition)
				{
					m_outputStream << utils::Keyword("precondition") << "(";
					operator_.printPredicateAsASP(m_outputStream);
					m_outputStream << ", ";
					precondition.variable().printNameAsASPPredicate(m_outputStream);
					m_outputStream << ", ";
					precondition.value().printAsASPPredicate(m_outputStream);
					m_outputStream << ")." << std::endl;
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
							m_outputStream << utils::Keyword("effectCondition") << "(";
							operator_.printPredicateAsASP(m_outputStream);
							m_outputStream << ", " << utils::Keyword("effect") << "(" << utils::Number(std::to_string(currentEffectID)) << "), ";
							condition.variable().printNameAsASPPredicate(m_outputStream);
							m_outputStream << ", ";
							condition.value().printAsASPPredicate(m_outputStream);
							m_outputStream << ")." << std::endl;
						});

					m_outputStream << utils::Keyword("postcondition") << "(";
					operator_.printPredicateAsASP(m_outputStream);
					m_outputStream << ", " << utils::Keyword("effect") << "(" << utils::Number(std::to_string(currentEffectID)) << "), ";
					effect.postcondition().variable().printNameAsASPPredicate(m_outputStream);
					m_outputStream << ", ";
					effect.postcondition().value().printAsASPPredicate(m_outputStream);
					m_outputStream << ")." << std::endl;

					currentEffectID++;
				});

			m_outputStream << utils::Keyword("costs") << "(";
			operator_.printPredicateAsASP(m_outputStream);
			m_outputStream << ", " << operator_.costs() << ")." << std::endl;
		});

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateMutexes() const
{
	m_outputStream << utils::Heading2("mutex groups");

	const auto &mutexGroups = m_description.mutexGroups();

	size_t currentMutexGroupID = 0;

	std::for_each(mutexGroups.cbegin(), mutexGroups.cend(),
		[&](const auto &mutexGroup)
		{
			const auto mutexGroupID = std::to_string(currentMutexGroupID);
			currentMutexGroupID++;

			m_outputStream << std::endl << utils::Keyword("mutexGroup") << "(" << utils::Number(mutexGroupID) << ")." << std::endl;

			const auto &facts = mutexGroup.facts();

			std::for_each(facts.cbegin(), facts.cend(),
				[&](const auto &fact)
				{
					m_outputStream << utils::Keyword("contains") << "(" << utils::Keyword("mutexGroup") << "(" << utils::Number(mutexGroupID) << "), ";
					fact.variable().printNameAsASPPredicate(m_outputStream);
					m_outputStream << ", ";
					fact.value().printAsASPPredicate(m_outputStream);
					m_outputStream << ")." << std::endl;
				});
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateAxiomRules() const
{
	if (!m_description.usesAxiomRules())
		return;

	m_outputStream << std::endl;
	m_outputStream << utils::Heading2("axiom rules");

	const auto &axiomRules = m_description.axiomRules();

	size_t currentAxiomRuleID = 0;

	std::for_each(axiomRules.cbegin(), axiomRules.cend(),
		[&](const auto &axiomRule)
		{
			const auto axiomRuleID = std::to_string(currentAxiomRuleID);
			currentAxiomRuleID++;

			m_outputStream << std::endl << utils::Keyword("axiomRule") << "(" << utils::Number(axiomRuleID) << ")." << std::endl;

			const auto &conditions = axiomRule.conditions();

			std::for_each(conditions.cbegin(), conditions.cend(),
				[&](const auto &condition)
				{
					m_outputStream << utils::Keyword("condition") << "(" << utils::Keyword("axiomRule") << "(" << utils::Number(axiomRuleID) << "), ";
					condition.variable().printNameAsASPPredicate(m_outputStream);
					m_outputStream << ", ";
					condition.value().printAsASPPredicate(m_outputStream);
					m_outputStream << ")." << std::endl;
				});

			const auto &postcondition = axiomRule.postcondition();

			m_outputStream << utils::Keyword("postcondition") << "(" << utils::Keyword("axiomRule") << "(" << utils::Number(axiomRuleID) << "), ";
			postcondition.variable().printNameAsASPPredicate(m_outputStream);
			m_outputStream << ", ";
			postcondition.value().printAsASPPredicate(m_outputStream);
			m_outputStream << ")." << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
