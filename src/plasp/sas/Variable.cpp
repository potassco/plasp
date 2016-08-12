#include <plasp/sas/Variable.h>

#include <iostream>

#include <plasp/utils/Formatting.h>
#include <plasp/utils/IO.h>
#include <plasp/utils/ParserException.h>

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

Variable Variable::fromSAS(utils::Parser<> &parser)
{
	Variable variable;

	parser.expect<std::string>("begin_variable");
	parser.expect<std::string>("var");

	variable.m_name = parser.parse<std::string>();
	variable.m_axiomLayer = parser.parse<int>();

	const auto numberOfValues = parser.parse<size_t>();
	variable.m_values.reserve(numberOfValues);

	for (size_t j = 0; j < numberOfValues; j++)
	{
		variable.m_values.emplace_back(Value::fromSAS(parser));

		// <none of those> values are only allowed at the end
		if (j < numberOfValues - 1 && variable.m_values[j] == Value::None)
			throw utils::ParserException(parser.coordinate(), "<none of those> value must be the last value of a variable");
	}

	parser.expect<std::string>("end_variable");

	return variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::printNameAsASPPredicate(utils::LogStream &outputStream) const
{
	outputStream << utils::Keyword("variable") << "(" << utils::escapeASP(m_name) << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable &Variable::referenceFromSAS(utils::Parser<> &parser, const Variables &variables)
{
	const auto variableID = parser.parse<size_t>();

	if (variableID >= variables.size())
		throw utils::ParserException(parser.coordinate(), "variable index out of range (index " + std::to_string(variableID) + ")");

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
