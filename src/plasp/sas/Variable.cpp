#include <plasp/sas/Variable.h>

#include <iostream>

#include <plasp/output/Formatting.h>

#include <parsebase/ParserException.h>

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

Variable Variable::fromSAS(parsebase::Parser<> &parser)
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
			throw parsebase::ParserException(parser.location(), "<none of those> value must be the last value of a variable");
	}

	parser.expect<std::string>("end_variable");

	return variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::printNameAsASPPredicate(output::ColorStream &stream) const
{
	// TODO: assert that name is a number indeed
	stream << output::Keyword("variable") << "(" << output::Number<std::string>(m_name) << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable &Variable::referenceFromSAS(parsebase::Parser<> &parser, const Variables &variables)
{
	const auto variableID = parser.parse<size_t>();

	if (variableID >= variables.size())
		throw parsebase::ParserException(parser.location(), "variable index out of range (index " + std::to_string(variableID) + ")");

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
