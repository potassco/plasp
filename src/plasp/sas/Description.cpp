#include <plasp/sas/Description.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/filesystem.hpp>

#include <plasp/utils/ParserException.h>
#include <plasp/utils/Parsing.h>
#include <plasp/sas/VariableTransition.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Description::Description()
:	m_usesActionCosts{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromStream(std::istream &istream)
{
	Description description;

	std::setlocale(LC_NUMERIC, "C");

	istream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	description.parseVersionSection(istream);
	description.parseMetricSection(istream);
	description.parseVariablesSection(istream);
	description.parseMutexSection(istream);
	description.parseInitialStateSection(istream);
	description.parseGoalSection(istream);
	description.parseOperatorSection(istream);
	description.parseAxiomSection(istream);

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromFile(const boost::filesystem::path &path)
{
	if (!boost::filesystem::is_regular_file(path))
		throw std::runtime_error("File does not exist: \"" + path.string() + "\"");

	std::ifstream fileStream(path.string(), std::ios::in);

	return Description::fromStream(fileStream);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Description::usesActionCosts() const
{
	return m_usesActionCosts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variables &Description::variables() const
{
	return m_variables;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const MutexGroups &Description::mutexGroups() const
{
	return m_mutexGroups;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const InitialState &Description::initialState() const
{
	return *m_initialState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Goal &Description::goal() const
{
	return *m_goal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Operators &Description::operators() const
{
	return m_operators;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const AxiomRules &Description::axiomRules() const
{
	return m_axiomRules;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::print(std::ostream &ostream) const
{
	// Metric section
	ostream << "uses action costs: " << (m_usesActionCosts ? "yes" : "no") << std::endl;

	// Variable section
	ostream << "variables: " << m_variables.size() << std::endl;

	std::for_each(m_variables.cbegin(), m_variables.cend(),
		[&](const auto &variable)
		{
			const auto &values = variable.values();

			ostream << "\t" << variable.name() << ":" << std::endl;
			ostream << "\t\tvalues: " << values.size() << std::endl;

			std::for_each(values.cbegin(), values.cend(),
				[&](const auto &value)
			    {
					ostream << "\t\t\t";
					value.printAsSAS(ostream);
					ostream << std::endl;
				});

			ostream << "\t\taxiom layer: " << variable.axiomLayer() << std::endl;
		});

	// Mutex section
	ostream << "mutex groups: " << m_mutexGroups.size() << std::endl;

	std::for_each(m_mutexGroups.cbegin(), m_mutexGroups.cend(),
		[&](const auto &mutexGroup)
		{
			ostream << "\tmutex group:" << std::endl;

			std::for_each(mutexGroup.facts().cbegin(), mutexGroup.facts().cend(),
				[&](const auto &fact)
				{
					ostream << "\t\t" << fact.variable().name() << " = ";
					fact.value().printAsSAS(ostream);
					ostream << std::endl;
				});
		});

	// Initial state section
	ostream << "initial state:" << std::endl;

	std::for_each(m_initialState->facts().cbegin(), m_initialState->facts().cend(),
		[&](const auto &fact)
		{
			ostream << "\t" << fact.variable().name() << " = ";
			fact.value().printAsSAS(ostream);
			ostream << std::endl;
		});

	// Goal section
	ostream << "goal:" << std::endl;

	std::for_each(m_goal->facts().cbegin(), m_goal->facts().cend(),
		[&](const auto &fact)
		{
			ostream << "\t" << fact.variable().name() << " = ";
			fact.value().printAsSAS(ostream);
			ostream << std::endl;
		});

	// Operator section
	ostream << "operators: " << m_operators.size() << std::endl;

	std::for_each(m_operators.cbegin(), m_operators.cend(),
		[&](const auto &operator_)
		{
			ostream << "\t" << operator_.predicate() << ":" << std::endl;

			const auto &preconditions = operator_.preconditions();

			ostream << "\t\tpreconditions: " << preconditions.size() << std::endl;

			std::for_each(preconditions.cbegin(), preconditions.cend(),
				[&](const auto &precondition)
				{
					std::cout << "\t\t\t" << precondition.variable().name() << " = ";
					precondition.value().printAsSAS(ostream);
					ostream << std::endl;
				});

			const auto &effects = operator_.effects();

			ostream << "\t\teffects: " << effects.size() << std::endl;

			std::for_each(effects.cbegin(), effects.cend(),
				[&](const auto &effect)
				{
					ostream << "\t\t\teffect:" << std::endl;
					ostream << "\t\t\t\tconditions: " << effect.conditions.size() << std::endl;

					std::for_each(effect.conditions.cbegin(), effect.conditions.cend(),
						[&](const auto &condition)
						{
							ostream << "\t\t\t\t\t" << condition.variable().name() << " = ";
							condition.value().printAsSAS(ostream);
							ostream << std::endl;
						});

					ostream << "\t\t\t\tpostcondition:" << std::endl;
					ostream << "\t\t\t\t\t" << effect.postcondition.variable().name() << " = ";
					effect.postcondition.value().printAsSAS(ostream);
					ostream << std::endl;
				});

			ostream << "\t\tcosts: " << operator_.costs() << std::endl;
		});

	// Axiom section
	ostream << "axiom rules: " << m_axiomRules.size() << std::endl;

	std::for_each(m_axiomRules.cbegin(), m_axiomRules.cend(),
		[&](const auto &axiomRule)
		{
			ostream << "\taxiom rule:" << std::endl;
			ostream << "\t\tconditions: " << axiomRule.conditions.size() << std::endl;

			std::for_each(axiomRule.conditions.cbegin(), axiomRule.conditions.cend(),
				[&](const auto &condition)
				{
					ostream << "\t\t\t" << condition.variable().name() << " = ";
					condition.value().printAsSAS(ostream);
					ostream << std::endl;
				});

			ostream << "\t\tpostcondition:" << std::endl;
			ostream << "\t\t\t" << axiomRule.postcondition.variable().name() << " = ";
			axiomRule.postcondition.value().printAsSAS(ostream);
			ostream << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseVersionSection(std::istream &istream) const
{
	// Version section
	utils::parseExpected<std::string>(istream, "begin_version");

	const auto formatVersion = utils::parse<size_t>(istream);

	if (formatVersion != 3)
		throw utils::ParserException("Unsupported SAS format version (" + std::to_string(formatVersion) + ")");

	utils::parseExpected<std::string>(istream, "end_version");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseMetricSection(std::istream &istream)
{
	utils::parseExpected<std::string>(istream, "begin_metric");

	m_usesActionCosts = utils::parse<bool>(istream);

	utils::parseExpected<std::string>(istream, "end_metric");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseVariablesSection(std::istream &istream)
{
	const auto numberOfVariables = utils::parse<size_t>(istream);
	m_variables.reserve(numberOfVariables);

	for (size_t i = 0; i < numberOfVariables; i++)
		m_variables.emplace_back(Variable::fromSAS(istream));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseMutexSection(std::istream &istream)
{
	const auto numberOfMutexGroups = utils::parse<size_t>(istream);
	m_mutexGroups.reserve(numberOfMutexGroups);

	for (size_t i = 0; i < numberOfMutexGroups; i++)
		m_mutexGroups.emplace_back(MutexGroup::fromSAS(istream, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseInitialStateSection(std::istream &istream)
{
	m_initialState = std::make_unique<InitialState>(InitialState::fromSAS(istream, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseGoalSection(std::istream &istream)
{
	m_goal = std::make_unique<Goal>(Goal::fromSAS(istream, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseOperatorSection(std::istream &istream)
{
	const auto numberOfOperators = utils::parse<size_t>(istream);
	m_operators.reserve(numberOfOperators);

	for (size_t i = 0; i < numberOfOperators; i++)
		m_operators.emplace_back(Operator::fromSAS(istream, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseAxiomSection(std::istream &istream)
{
	const auto numberOfAxiomRules = utils::parse<size_t>(istream);
	m_axiomRules.reserve(numberOfAxiomRules);

	for (size_t i = 0; i < numberOfAxiomRules; i++)
	{
		utils::parseExpected<std::string>(istream, "begin_rule");

		const auto numberOfConditions = utils::parse<size_t>(istream);

		AxiomRule::Conditions conditions;
		conditions.reserve(numberOfConditions);

		for (size_t j = 0; j < numberOfConditions; j++)
			conditions.emplace_back(AssignedVariable::fromSAS(istream, m_variables));

		const auto variableTransition = VariableTransition::fromSAS(istream, m_variables);

		if (&variableTransition.valueBefore() != &Value::Any)
			conditions.emplace_back(AssignedVariable(variableTransition.variable(), variableTransition.valueBefore()));

		const AxiomRule::Condition postcondition = {variableTransition.variable(), variableTransition.valueAfter()};
		const AxiomRule axiomRule = {std::move(conditions), std::move(postcondition)};
		m_axiomRules.push_back(std::move(axiomRule));

		utils::parseExpected<std::string>(istream, "end_rule");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
