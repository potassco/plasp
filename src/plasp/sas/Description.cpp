#include <plasp/sas/Description.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include <tokenize/TokenizerException.h>

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

Description Description::fromTokenizer(tokenize::Tokenizer<> &&tokenizer)
{
	Description description;
	description.parseContent(tokenizer);

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromStream(std::istream &istream)
{
	tokenize::Tokenizer<> tokenizer;
	tokenizer.read("std::cin", istream);

	Description description;
	description.parseContent(tokenizer);

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromFile(const std::experimental::filesystem::path &path)
{
	if (!std::experimental::filesystem::is_regular_file(path))
		throw std::runtime_error("File does not exist: “" + path.string() + "”");

	tokenize::Tokenizer<> tokenizer;
	tokenizer.read(path);

	Description description;
	description.parseContent(tokenizer);

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

void Description::parseContent(tokenize::Tokenizer<> &tokenizer)
{
	parseVersionSection(tokenizer);
	parseMetricSection(tokenizer);
	parseVariablesSection(tokenizer);
	parseMutexSection(tokenizer);
	parseInitialStateSection(tokenizer);
	parseGoalSection(tokenizer);
	parseOperatorSection(tokenizer);
	parseAxiomSection(tokenizer);

	tokenizer.skipWhiteSpace();

	if (!tokenizer.atEnd())
		throw tokenize::TokenizerException(tokenizer.location(), "expected end of SAS description (perhaps, input contains two SAS descriptions?)");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseVersionSection(tokenize::Tokenizer<> &tokenizer) const
{
	tokenizer.expect<std::string>("begin_version");

	const auto formatVersion = tokenizer.get<size_t>();

	if (formatVersion != 3)
		throw tokenize::TokenizerException(tokenizer.location(), "unsupported SAS format version (" + std::to_string(formatVersion) + ")");

	tokenizer.expect<std::string>("end_version");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseMetricSection(tokenize::Tokenizer<> &tokenizer)
{
	tokenizer.expect<std::string>("begin_metric");

	m_usesActionCosts = tokenizer.get<bool>();

	tokenizer.expect<std::string>("end_metric");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseVariablesSection(tokenize::Tokenizer<> &tokenizer)
{
	const auto numberOfVariables = tokenizer.get<size_t>();
	m_variables.reserve(numberOfVariables);

	for (size_t i = 0; i < numberOfVariables; i++)
		m_variables.emplace_back(Variable::fromSAS(tokenizer));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseMutexSection(tokenize::Tokenizer<> &tokenizer)
{
	const auto numberOfMutexGroups = tokenizer.get<size_t>();
	m_mutexGroups.reserve(numberOfMutexGroups);

	for (size_t i = 0; i < numberOfMutexGroups; i++)
		m_mutexGroups.emplace_back(MutexGroup::fromSAS(tokenizer, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseInitialStateSection(tokenize::Tokenizer<> &tokenizer)
{
	m_initialState = std::make_unique<InitialState>(InitialState::fromSAS(tokenizer, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseGoalSection(tokenize::Tokenizer<> &tokenizer)
{
	m_goal = std::make_unique<Goal>(Goal::fromSAS(tokenizer, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseOperatorSection(tokenize::Tokenizer<> &tokenizer)
{
	const auto numberOfOperators = tokenizer.get<size_t>();
	m_operators.reserve(numberOfOperators);

	for (size_t i = 0; i < numberOfOperators; i++)
		m_operators.emplace_back(Operator::fromSAS(tokenizer, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseAxiomSection(tokenize::Tokenizer<> &tokenizer)
{
	const auto numberOfAxiomRules = tokenizer.get<size_t>();
	m_axiomRules.reserve(numberOfAxiomRules);

	for (size_t i = 0; i < numberOfAxiomRules; i++)
		m_axiomRules.emplace_back(AxiomRule::fromSAS(tokenizer, m_variables));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
