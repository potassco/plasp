#include <plasp/sas/Value.h>

#include <iostream>

#include <plasp/sas/Variable.h>
#include <plasp/utils/Parsing.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Value
//
////////////////////////////////////////////////////////////////////////////////////////////////////

const Value Value::any()
{
	Value any;

	any.m_name = "(any)";

	return any;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value Value::Any = Value::any();

////////////////////////////////////////////////////////////////////////////////////////////////////

Value::Value()
:	m_sign{Sign::Positive},
	m_hasArguments{true}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Value Value::fromSAS(std::istream &istream)
{
	Value value;

	const auto sasSign = utils::parse<std::string>(istream);

	if (sasSign == "Atom")
		value.m_sign = Value::Sign::Positive;
	else if (sasSign == "NegatedAtom")
		value.m_sign = Value::Sign::Negative;
	else
		throw utils::ParserException("Invalid value sign \"" + sasSign + "\"");

	try
	{
		istream.ignore(1);
		std::getline(istream, value.m_name);

		// Remove trailing ()
		if (value.m_name.find("()") != std::string::npos)
		{
			value.m_hasArguments = false;
			value.m_name.resize(value.m_name.size() - 2);
		}
	}
	catch (const std::exception &e)
	{
		throw utils::ParserException(std::string("Could not parse variable value (") + e.what() + ")");
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value &Value::referenceFromSAS(std::istream &istream, const Variable &variable)
{
	const auto valueID = utils::parse<int>(istream);

	if (valueID == -1)
		return Value::Any;

	if (valueID < 0 || static_cast<size_t>(valueID) >= variable.values().size())
		throw utils::ParserException("Value index out of range (variable " + variable.name() + ", index " + std::to_string(valueID) + ")");

	return variable.values()[valueID];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Value::Sign Value::sign() const
{
	return m_sign;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Value::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Value::printAsASP(std::ostream &ostream) const
{
	if (m_sign == Value::Sign::Negative)
		ostream << "not ";

	ostream << m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Value::printAsASPCommaSeparated(std::ostream &ostream) const
{
	ostream << m_name << ", " << (m_sign == Sign::Positive ? "true" : "false");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Value::printAsSAS(std::ostream &ostream) const
{
	if (m_sign == Value::Sign::Positive)
		ostream << "Atom ";
	else
		ostream << "NegatedAtom ";

	ostream << m_name;

	if (!m_hasArguments)
		ostream << "()";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
