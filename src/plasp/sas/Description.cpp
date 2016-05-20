#include <plasp/sas/Description.h>

#include <iostream>

#include <boost/filesystem.hpp>

#include <plasp/sas/ParserException.h>

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

	setlocale(LC_NUMERIC, "C");

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

const std::vector<Variable> &Description::variables() const
{
	return m_variables;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<MutexGroup> &Description::mutexGroups() const
{
	return m_mutexGroups;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<AssignedVariable> &Description::initialStateFacts() const
{
	return m_initialStateFacts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<AssignedVariable> &Description::goalFacts() const
{
	return m_goalFacts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<Operator> &Description::operators() const
{
	return m_operators;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<AxiomRule> &Description::axiomRules() const
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
			ostream << "\t" << variable.name << ":" << std::endl;
			ostream << "\t\tvalues: " << variable.values.size() << std::endl;

			std::for_each(variable.values.cbegin(), variable.values.cend(),
				[&](const auto &value)
			    {
					ostream << "\t\t\t" << value.name << std::endl;
				});

			ostream << "\t\taxiom layer: " << variable.axiomLayer << std::endl;
		});

	// Mutex section
	ostream << "mutex groups: " << m_mutexGroups.size() << std::endl;

	std::for_each(m_mutexGroups.cbegin(), m_mutexGroups.cend(),
		[&](const auto &mutexGroup)
		{
			ostream << "\tmutex group:" << std::endl;

			std::for_each(mutexGroup.facts.cbegin(), mutexGroup.facts.cend(),
				[&](const auto &fact)
				{
					ostream << "\t\t" << fact.variable.name << " = " << fact.value.name << std::endl;
				});
		});

	// Initial state section
	ostream << "initial state:" << std::endl;

	std::for_each(m_initialStateFacts.cbegin(), m_initialStateFacts.cend(),
		[&](const auto &initialStateFact)
		{
			ostream << "\t" << initialStateFact.variable.name << " = " << initialStateFact.value.name << std::endl;
		});

	// Goal section
	ostream << "goal:" << std::endl;

	std::for_each(m_goalFacts.cbegin(), m_goalFacts.cend(),
		[&](const auto &goalFact)
		{
			ostream << "\t" << goalFact.variable.name << " = " << goalFact.value.name << std::endl;
		});

	// Operator section
	ostream << "operators: " << m_operators.size() << std::endl;

	std::for_each(m_operators.cbegin(), m_operators.cend(),
		[&](const auto &operator_)
		{
			ostream << "\t" << operator_.name << ":" << std::endl;
			ostream << "\t\tpreconditions: " << operator_.preconditions.size() << std::endl;

			std::for_each(operator_.preconditions.cbegin(), operator_.preconditions.cend(),
				[&](const auto &precondition)
				{
					std::cout << "\t\t\t" << precondition.variable.name << " = " << precondition.value.name << std::endl;
				});

			ostream << "\t\teffects: " << operator_.effects.size() << std::endl;

			std::for_each(operator_.effects.cbegin(), operator_.effects.cend(),
				[&](const auto &effect)
				{
					ostream << "\t\t\teffect:" << std::endl;
					ostream << "\t\t\t\tconditions: " << effect.conditions.size() << std::endl;

					std::for_each(effect.conditions.cbegin(), effect.conditions.cend(),
						[&](const auto &condition)
						{
							ostream << "\t\t\t\t\t" << condition.variable.name << " = " << condition.value.name << std::endl;
						});

					ostream << "\t\t\t\tpostcondition:" << std::endl;
					ostream << "\t\t\t\t\t" << effect.postcondition.variable.name << " = " << effect.postcondition.value.name << std::endl;
				});

			ostream << "\t\tcosts: " << operator_.costs << std::endl;
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
					ostream << "\t\t\t" << condition.variable.name << " = " << condition.value.name << std::endl;
				});

			ostream << "\t\tpostcondition:" << std::endl;
			ostream << "\t\t\t" << axiomRule.postcondition.variable.name << " = " << axiomRule.postcondition.value.name << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseSectionIdentifier(std::istream &istream, const std::string &expectedSectionIdentifier) const
{
	const auto sectionIdentifier = parse<std::string>(istream);

	if (sectionIdentifier != expectedSectionIdentifier)
		throw ParserException("Invalid format, expected " + expectedSectionIdentifier + ", got " + sectionIdentifier);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
T Description::parse(std::istream &istream) const
{
	T value;

	try
	{
		istream >> value;
	}
	catch (const std::exception &e)
	{
		throw ParserException(std::string("Could not parse value of type ") + typeid(T).name() + " (" + e.what() + ")");
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable &Description::parseVariable(std::istream &istream) const
{
	const auto variableID = parse<size_t>(istream);

	if (variableID >= m_variables.size())
		throw ParserException("Variable index out of range (index " + std::to_string(variableID) + ")");

	return m_variables[variableID];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value &Description::parseValue(std::istream &istream, const Variable &variable) const
{
	const auto valueID = parse<int>(istream);

	if (valueID == -1)
		return Value::Any;

	if (valueID < 0 || static_cast<size_t>(valueID) >= variable.values.size())
		throw ParserException("Value index out of range (variable " + variable.name + ", index " + std::to_string(valueID) + ")");

	return variable.values[valueID];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AssignedVariable Description::parseAssignedVariable(std::istream &istream) const
{
	const auto &variable = parseVariable(istream);
	const auto &value = parseValue(istream, variable);

	return {variable, value};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VariableTransition Description::parseVariableTransition(std::istream &istream) const
{
	const auto &variable = parseVariable(istream);
	const auto &valueBefore = parseValue(istream, variable);
	const auto &valueAfter = parseValue(istream, variable);

	return {variable, valueBefore, valueAfter};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseVersionSection(std::istream &istream) const
{
	// Version section
	parseSectionIdentifier(istream, "begin_version");

	const auto formatVersion = parse<size_t>(istream);

	if (formatVersion != 3)
		throw ParserException("Unsupported SAS format version (" + std::to_string(formatVersion) + ")");

	parseSectionIdentifier(istream, "end_version");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseMetricSection(std::istream &istream)
{
	parseSectionIdentifier(istream, "begin_metric");

	m_usesActionCosts = parse<bool>(istream);

	parseSectionIdentifier(istream, "end_metric");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseVariablesSection(std::istream &istream)
{
	const auto numberOfVariables = parse<size_t>(istream);
	m_variables.resize(numberOfVariables);

	for (size_t i = 0; i < numberOfVariables; i++)
	{
		auto &variable = m_variables[i];

		parseSectionIdentifier(istream, "begin_variable");

		variable.name = parse<std::string>(istream);
		variable.axiomLayer = parse<int>(istream);

		const auto numberOfValues = parse<size_t>(istream);
		variable.values.resize(numberOfValues);

		try
		{
			istream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			for (size_t j = 0; j < numberOfValues; j++)
			{
				auto &value = variable.values[j];
				std::getline(istream, value.name);
			}
		}
		catch (const std::exception &e)
		{
			throw ParserException("Could not parse variable " + variable.name + " (" + e.what() + ")");
		}

		parseSectionIdentifier(istream, "end_variable");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseMutexSection(std::istream &istream)
{
	const auto numberOfMutexGroups = parse<size_t>(istream);
	m_mutexGroups.resize(numberOfMutexGroups);

	for (size_t i = 0; i < numberOfMutexGroups; i++)
	{
		parseSectionIdentifier(istream, "begin_mutex_group");

		auto &mutexGroup = m_mutexGroups[i];

		const auto numberOfFacts = parse<size_t>(istream);
		mutexGroup.facts.reserve(numberOfFacts);

		for (size_t j = 0; j < numberOfFacts; j++)
		{
			const auto fact = parseAssignedVariable(istream);
			mutexGroup.facts.push_back(std::move(fact));
		}

		parseSectionIdentifier(istream, "end_mutex_group");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseInitialStateSection(std::istream &istream)
{
	parseSectionIdentifier(istream, "begin_state");

	m_initialStateFacts.reserve(m_variables.size());

	for (size_t i = 0; i < m_variables.size(); i++)
	{
		const auto &variable = m_variables[i];
		const auto &value = parseValue(istream, variable);

		m_initialStateFacts.push_back({variable, value});
	}

	parseSectionIdentifier(istream, "end_state");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseGoalSection(std::istream &istream)
{
	parseSectionIdentifier(istream, "begin_goal");

	const auto numberOfGoalFacts = parse<size_t>(istream);
	m_goalFacts.reserve(numberOfGoalFacts);

	for (size_t i = 0; i < numberOfGoalFacts; i++)
	{
		const auto goalFact = parseAssignedVariable(istream);
		m_goalFacts.push_back(std::move(goalFact));
	}

	parseSectionIdentifier(istream, "end_goal");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseOperatorSection(std::istream &istream)
{
	const auto numberOfOperators = parse<size_t>(istream);
	m_operators.resize(numberOfOperators);

	for (size_t i = 0; i < numberOfOperators; i++)
	{
		parseSectionIdentifier(istream, "begin_operator");

		istream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		auto &operator_ = m_operators[i];
		std::getline(istream, operator_.name);

		const auto numberOfPrevailConditions = parse<size_t>(istream);
		operator_.preconditions.reserve(numberOfPrevailConditions);

		for (size_t j = 0; j < numberOfPrevailConditions; j++)
		{
			const auto precondition = parseAssignedVariable(istream);
			operator_.preconditions.push_back(std::move(precondition));
		}

		const auto numberOfEffects = parse<size_t>(istream);
		operator_.effects.reserve(numberOfEffects);

		for (size_t j = 0; j < numberOfEffects; j++)
		{
			Effect::Conditions conditions;

			const auto numberOfEffectConditions = parse<size_t>(istream);
			conditions.reserve(numberOfEffectConditions);

			for (size_t k = 0; k < numberOfEffectConditions; k++)
			{
				const auto condition = parseAssignedVariable(istream);
				conditions.push_back(std::move(condition));
			}

			const auto variableTransition = parseVariableTransition(istream);

			if (&variableTransition.valueBefore != &Value::Any)
				operator_.preconditions.push_back({variableTransition.variable, variableTransition.valueBefore});

			const Effect::Condition postcondition = {variableTransition.variable, variableTransition.valueAfter};
			const Effect effect = {std::move(conditions), std::move(postcondition)};
			operator_.effects.push_back(std::move(effect));
		}

		operator_.costs = parse<size_t>(istream);

		parseSectionIdentifier(istream, "end_operator");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseAxiomSection(std::istream &istream)
{
	const auto numberOfAxiomRules = parse<size_t>(istream);
	m_axiomRules.reserve(numberOfAxiomRules);

	for (size_t i = 0; i < numberOfAxiomRules; i++)
	{
		parseSectionIdentifier(istream, "begin_rule");

		const auto numberOfConditions = parse<size_t>(istream);

		AxiomRule::Conditions conditions;
		conditions.reserve(numberOfConditions);

		for (size_t j = 0; j < numberOfConditions; j++)
		{
			const auto condition = parseAssignedVariable(istream);
			conditions.push_back(std::move(condition));
		}

		const auto variableTransition = parseVariableTransition(istream);

		if (&variableTransition.valueBefore != &Value::Any)
			conditions.push_back({variableTransition.variable, variableTransition.valueBefore});

		const AxiomRule::Condition postcondition = {variableTransition.variable, variableTransition.valueAfter};
		const AxiomRule axiomRule = {std::move(conditions), std::move(postcondition)};
		m_axiomRules.push_back(std::move(axiomRule));

		parseSectionIdentifier(istream, "end_rule");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
