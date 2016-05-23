#include <plasp/sas/Variable.h>

#include <iostream>

#include <plasp/utils/Parsing.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Variable::Variable()
:	m_axiomLayer(-1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Variable Variable::fromSAS(std::istream &istream)
{
	Variable variable;

	utils::parseExpected<std::string>(istream, "begin_variable");

	variable.m_name = utils::parse<std::string>(istream);
	variable.m_axiomLayer = utils::parse<int>(istream);

	const auto numberOfValues = utils::parse<size_t>(istream);
	variable.m_values.reserve(numberOfValues);

	for (size_t j = 0; j < numberOfValues; j++)
	{
		variable.m_values.emplace_back(Value::fromSAS(istream));

		// <none of those> values are only allowed at the end
		if (j < numberOfValues - 1 && variable.m_values[j] == Value::None)
			throw utils::ParserException("<none of those> value must be the last value of a variable");
	}

	utils::parseExpected<std::string>(istream, "end_variable");

	return variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::printNameAsASP(std::ostream &ostream) const
{
	ostream << utils::escapeASP(m_name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable &Variable::referenceFromSAS(std::istream &istream, const Variables &variables)
{
	const auto variableID = utils::parse<size_t>(istream);

	if (variableID >= variables.size())
		throw utils::ParserException("Variable index out of range (index " + std::to_string(variableID) + ")");

	return variables[variableID];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Variable::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Variable::axiomLayer() const
{
	return m_axiomLayer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Values &Variable::values() const
{
	return m_values;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
