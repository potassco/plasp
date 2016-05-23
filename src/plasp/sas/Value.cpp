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

const Value Value::reserved(const std::string &name)
{
	Value any;

	any.m_name = name;

	return any;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value Value::Any = Value::reserved("(any)");
const Value Value::None = Value::reserved("(none)");

////////////////////////////////////////////////////////////////////////////////////////////////////

Value::Value()
:	m_sign{Sign::Positive},
	m_hasArguments{true}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Value Value::negated() const
{
	Value negated;

	negated.m_sign = (m_sign == Sign::Positive ? Sign::Negative : Sign::Positive);
	negated.m_name = m_name;
	negated.m_hasArguments = m_hasArguments;

	return negated;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Value Value::fromSAS(std::istream &istream)
{
	const auto sasSign = utils::parse<std::string>(istream);

	if (sasSign == "<none")
	{
		utils::parseExpected<std::string>(istream, "of");
		utils::parseExpected<std::string>(istream, "those>");

		// TODO: do not return a copy of Value::None
		return Value::None;
	}

	Value value;

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

	ostream << utils::escapeASP(m_name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Value::printAsASPPredicate(std::ostream &ostream) const
{
	ostream << "value(" << utils::escapeASP(m_name) << ", "
		<< (m_sign == Sign::Positive ? "true" : "false") << ")";
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

bool operator ==(const Value &value1, const Value &value2)
{
	if (value1.sign() != value2.sign())
		return false;

	if (value1.name() != value2.name())
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
