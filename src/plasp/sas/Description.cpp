#include <plasp/sas/Description.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/filesystem.hpp>

#include <plasp/input/ParserException.h>
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

Description Description::fromParser(input::Parser<> &&parser)
{
	Description description;
	description.parseContent(parser);

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromStream(std::istream &istream)
{
	input::Parser<> parser;
	parser.read("std::cin", istream);

	Description description;
	description.parseContent(parser);

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromFile(const boost::filesystem::path &path)
{
	if (!boost::filesystem::is_regular_file(path))
		throw std::runtime_error("File does not exist: “" + path.string() + "”");

	input::Parser<> parser;
	parser.read(path);

	Description description;
	description.parseContent(parser);

	return description;
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

bool Description::usesAxiomRules() const
{
	if (!m_axiomRules.empty())
		return true;

	const auto match = std::find_if(m_variables.cbegin(), m_variables.cend(),
		[&](const auto &variable)
		{
			return variable.axiomLayer() != -1;
		});

	return match != m_variables.cend();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Description::usesConditionalEffects() const
{
	const auto match = std::find_if(m_operators.cbegin(), m_operators.cend(),
		[&](const auto &operator_)
		{
			const auto &effects = operator_.effects();

			const auto match = std::find_if(effects.cbegin(), effects.cend(),
				[&](const auto &effect)
				{
					return !effect.conditions().empty();
				});

			return match != effects.cend();
		});

	return match != m_operators.cend();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Description::hasRequirements() const
{
	return usesActionCosts() || usesAxiomRules() || usesConditionalEffects();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseContent(input::Parser<> &parser)
{
	parseVersionSection(parser);
	parseMetricSection(parser);
	parseVariablesSection(parser);
	parseMutexSection(parser);
	parseInitialStateSection(parser);
	parseGoalSection(parser);
	parseOperatorSection(parser);
	parseAxiomSection(parser);

	parser.skipWhiteSpace();

	if (!parser.atEnd())
		throw input::ParserException(parser.location(), "expected end of SAS description (perhaps, input contains two SAS descriptions?)");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseVersionSection(input::Parser<> &parser) const
{
	parser.expect<std::string>("begin_version");

	const auto formatVersion = parser.parse<size_t>();

	if (formatVersion != 3)
		throw input::ParserException(parser.location(), "unsupported SAS format version (" + std::to_string(formatVersion) + ")");

	parser.expect<std::string>("end_version");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseMetricSection(input::Parser<> &parser)
{
	parser.expect<std::string>("begin_metric");

	m_usesActionCosts = parser.parse<bool>();

	parser.expect<std::string>("end_metric");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseVariablesSection(input::Parser<> &parser)
{
	const auto numberOfVariables = parser.parse<size_t>();
	m_variables.reserve(numberOfVariables);

	for (size_t i = 0; i < numberOfVariables; i++)
		m_variables.emplace_back(Variable::fromSAS(parser));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseMutexSection(input::Parser<> &parser)
{
	const auto numberOfMutexGroups = parser.parse<size_t>();
	m_mutexGroups.reserve(numberOfMutexGroups);

	for (size_t i = 0; i < numberOfMutexGroups; i++)
		m_mutexGroups.emplace_back(MutexGroup::fromSAS(parser, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseInitialStateSection(input::Parser<> &parser)
{
	m_initialState = std::make_unique<InitialState>(InitialState::fromSAS(parser, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseGoalSection(input::Parser<> &parser)
{
	m_goal = std::make_unique<Goal>(Goal::fromSAS(parser, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseOperatorSection(input::Parser<> &parser)
{
	const auto numberOfOperators = parser.parse<size_t>();
	m_operators.reserve(numberOfOperators);

	for (size_t i = 0; i < numberOfOperators; i++)
		m_operators.emplace_back(Operator::fromSAS(parser, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseAxiomSection(input::Parser<> &parser)
{
	const auto numberOfAxiomRules = parser.parse<size_t>();
	m_axiomRules.reserve(numberOfAxiomRules);

	for (size_t i = 0; i < numberOfAxiomRules; i++)
		m_axiomRules.emplace_back(AxiomRule::fromSAS(parser, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
