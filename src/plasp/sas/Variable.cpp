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
	variable.m_values.resize(numberOfValues);

	try
	{
		for (size_t j = 0; j < numberOfValues; j++)
		{
			auto &value = variable.m_values[j];

			const auto sasSign = utils::parse<std::string>(istream);

			if (sasSign == "Atom")
				value.sign = Value::Sign::Positive;
			else if (sasSign == "NegatedAtom")
				value.sign = Value::Sign::Negative;
			else
				throw utils::ParserException("Invalid value sign \"" + sasSign + "\"");

			istream.ignore(1);

			std::getline(istream, value.name);
		}
	}
	catch (const std::exception &e)
	{
		throw utils::ParserException("Could not parse variable " + variable.m_name + " (" + e.what() + ")");
	}

	utils::parseExpected<std::string>(istream, "end_variable");

	return variable;
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
