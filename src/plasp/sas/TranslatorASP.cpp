#include <plasp/sas/TranslatorASP.h>

#include <colorlog/Formatting.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TranslatorASP
//
////////////////////////////////////////////////////////////////////////////////////////////////////

TranslatorASP::TranslatorASP(const Description &description, colorlog::ColorStream &outputStream)
:	m_description(description),
	m_outputStream(outputStream)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translate() const
{
	// TODO: remove double computation of requirements
	if (m_description.hasRequirements())
	{
		translateRequirements();
		m_outputStream << std::endl;
	}

	translateVariables();
	m_outputStream << std::endl;
	translateActions();

	if (!m_description.mutexGroups().empty())
	{
		m_outputStream << std::endl;
		translateMutexes();
	}

	if (m_description.usesAxiomRules())
	{
		m_outputStream << std::endl;
		translateAxiomRules();
	}

	m_outputStream << std::endl;
	translateInitialState();
	m_outputStream << std::endl;
	translateGoal();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateRequirements() const
{
	m_outputStream << colorlog::Heading2("feature requirements") << std::endl;

	if (m_description.usesActionCosts())
		m_outputStream << colorlog::Function("requires") << "(" << colorlog::Keyword("feature") << "(" << colorlog::Reserved("actionCosts") << "))." << std::endl;

	if (m_description.usesAxiomRules())
		m_outputStream << colorlog::Function("requires") << "(" << colorlog::Keyword("feature") << "(" << colorlog::Reserved("axiomRules") << "))." << std::endl;

	if (m_description.usesConditionalEffects())
		m_outputStream << colorlog::Function("requires") << "(" << colorlog::Keyword("feature") << "(" << colorlog::Reserved("conditionalEffects") << "))." << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateInitialState() const
{
	m_outputStream << colorlog::Heading2("initial state") << std::endl;

	const auto &initialStateFacts = m_description.initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			m_outputStream << colorlog::Function("initialState") << "(";
			fact.variable().printNameAsASPPredicate(m_outputStream);
			m_outputStream << ", ";
			fact.value().printAsASPPredicate(m_outputStream);
			m_outputStream << ")." << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateGoal() const
{
	m_outputStream << colorlog::Heading2("goal") << std::endl;

	const auto &goalFacts = m_description.goal().facts();

	std::for_each(goalFacts.cbegin(), goalFacts.cend(),
		[&](const auto &fact)
		{
			m_outputStream << colorlog::Function("goal") << "(";
			fact.variable().printNameAsASPPredicate(m_outputStream);
			m_outputStream << ", ";
			fact.value().printAsASPPredicate(m_outputStream);
			m_outputStream << ")." << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateVariables() const
{
	m_outputStream << colorlog::Heading2("variables");

	const auto &variables = m_description.variables();

	std::for_each(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			const auto &values = variable.values();

			assert(!values.empty());

			m_outputStream << std::endl << colorlog::Function("variable") << "(";
			variable.printNameAsASPPredicate(m_outputStream);
			m_outputStream << ")." << std::endl;

			std::for_each(values.cbegin(), values.cend(),
				[&](const auto &value)
				{
					m_outputStream << colorlog::Function("contains") << "(";
					variable.printNameAsASPPredicate(m_outputStream);
					m_outputStream << ", ";
					value.printAsASPPredicate(m_outputStream);
					m_outputStream << ")." << std::endl;
				});
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions() const
{
	m_outputStream << colorlog::Heading2("actions");

	const auto &operators = m_description.operators();

	size_t currentEffectID = 0;

	std::for_each(operators.cbegin(), operators.cend(),
		[&](const auto &operator_)
		{
			m_outputStream << std::endl << colorlog::Function("action") << "(";
			operator_.printPredicateAsASP(m_outputStream);
			m_outputStream << ")." << std::endl;

			const auto &preconditions = operator_.preconditions();

			std::for_each(preconditions.cbegin(), preconditions.cend(),
				[&](const auto &precondition)
				{
					m_outputStream << colorlog::Function("precondition") << "(";
					operator_.printPredicateAsASP(m_outputStream);
					m_outputStream << ", ";
					precondition.variable().printNameAsASPPredicate(m_outputStream);
					m_outputStream << ", ";
					precondition.value().printAsASPPredicate(m_outputStream);
					m_outputStream << ")." << std::endl;
				});

			const auto &effects = operator_.effects();

			std::for_each(effects.cbegin(), effects.cend(),
				[&](const auto &effect)
				{
					const auto &conditions = effect.conditions();

					m_outputStream << colorlog::Function("postcondition") << "(";
					operator_.printPredicateAsASP(m_outputStream);

					if (conditions.empty())
						m_outputStream << ", " << colorlog::Keyword("effect") << "(" << colorlog::Reserved("unconditional") << "), ";
					else
						m_outputStream << ", " << colorlog::Keyword("effect") << "(" << colorlog::Number<decltype(currentEffectID)>(currentEffectID) << "), ";

					effect.postcondition().variable().printNameAsASPPredicate(m_outputStream);
					m_outputStream << ", ";
					effect.postcondition().value().printAsASPPredicate(m_outputStream);
					m_outputStream << ")." << std::endl;

					std::for_each(conditions.cbegin(), conditions.cend(),
						[&](const auto &condition)
						{
							// Conditions of conditional effects
							m_outputStream
								<< colorlog::Function("precondition") << "("
								<< colorlog::Keyword("effect") << "(" << colorlog::Number<decltype(currentEffectID)>(currentEffectID) << "), ";
							condition.variable().printNameAsASPPredicate(m_outputStream);
							m_outputStream << ", ";
							condition.value().printAsASPPredicate(m_outputStream);
							m_outputStream << ")." << std::endl;
						});

					if (!conditions.empty())
						currentEffectID++;
				});

			m_outputStream << colorlog::Function("costs") << "(";
			operator_.printPredicateAsASP(m_outputStream);
			m_outputStream << ", " << colorlog::Number<decltype(operator_.costs())>(operator_.costs()) << ")." << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateMutexes() const
{
	m_outputStream << colorlog::Heading2("mutex groups");

	const auto &mutexGroups = m_description.mutexGroups();

	size_t currentMutexGroupID = 0;

	std::for_each(mutexGroups.cbegin(), mutexGroups.cend(),
		[&](const auto &mutexGroup)
		{
			const auto mutexGroupID = std::to_string(currentMutexGroupID);
			currentMutexGroupID++;

			m_outputStream
				<< std::endl
				<< colorlog::Function("mutexGroup") << "("
				<< colorlog::Keyword("mutexGroup") << "("
				<< colorlog::Number<decltype(mutexGroupID)>(mutexGroupID)
				<< "))." << std::endl;

			const auto &facts = mutexGroup.facts();

			std::for_each(facts.cbegin(), facts.cend(),
				[&](const auto &fact)
				{
					m_outputStream << colorlog::Function("contains") << "(" << colorlog::Keyword("mutexGroup") << "(" << colorlog::Number<decltype(mutexGroupID)>(mutexGroupID) << "), ";
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
	m_outputStream << colorlog::Heading2("axiom rules");

	const auto &axiomRules = m_description.axiomRules();

	size_t currentAxiomRuleID = 0;

	std::for_each(axiomRules.cbegin(), axiomRules.cend(),
		[&](const auto &axiomRule)
		{
			const auto axiomRuleID = std::to_string(currentAxiomRuleID);
			currentAxiomRuleID++;

			m_outputStream
				<< std::endl
				<< colorlog::Function("axiomRule") << "("
				<< colorlog::Keyword("axiomRule") << "("
				<< colorlog::Number<decltype(axiomRuleID)>(axiomRuleID)
				<< "))." << std::endl;

			// TODO: Translate axiom rule layer

			const auto &conditions = axiomRule.conditions();

			std::for_each(conditions.cbegin(), conditions.cend(),
				[&](const auto &condition)
				{
					m_outputStream
						<< colorlog::Function("precondition") << "("
						<< colorlog::Keyword("axiomRule") << "(" << colorlog::Number<decltype(axiomRuleID)>(axiomRuleID) << "), ";
					condition.variable().printNameAsASPPredicate(m_outputStream);
					m_outputStream << ", ";
					condition.value().printAsASPPredicate(m_outputStream);
					m_outputStream << ")." << std::endl;
				});

			const auto &postcondition = axiomRule.postcondition();

			m_outputStream
				<< colorlog::Function("postcondition") << "("
				<< colorlog::Keyword("axiomRule") << "(" << colorlog::Number<decltype(axiomRuleID)>(axiomRuleID) << "), "
				<< colorlog::Keyword("effect") << "(" << colorlog::Reserved("unconditional") << "), ";
			postcondition.variable().printNameAsASPPredicate(m_outputStream);
			m_outputStream << ", ";
			postcondition.value().printAsASPPredicate(m_outputStream);
			m_outputStream << ")." << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
