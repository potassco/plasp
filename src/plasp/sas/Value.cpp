#include <plasp/sas/Value.h>

#include <iostream>

#include <plasp/sas/Variable.h>
#include <plasp/utils/Formatting.h>
#include <plasp/utils/ParserException.h>

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

Value Value::fromSAS(utils::Parser<> &parser)
{
	const auto sasSign = parser.parse<std::string>();

	if (sasSign == "<none")
	{
		parser.expect<std::string>("of");
		parser.expect<std::string>("those>");

		// TODO: do not return a copy of Value::None
		return Value::None;
	}

	Value value;

	if (sasSign == "Atom")
		value.m_sign = Value::Sign::Positive;
	else if (sasSign == "NegatedAtom")
		value.m_sign = Value::Sign::Negative;
	else
		throw utils::ParserException(parser.coordinate(), "invalid value sign “" + sasSign + "”");

	try
	{
		parser.skipWhiteSpace();
		value.m_name = parser.parseLine();

		// Remove trailing ()
		if (value.m_name.find("()") != std::string::npos)
		{
			value.m_hasArguments = false;
			value.m_name.resize(value.m_name.size() - 2);
		}
	}
	catch (const std::exception &e)
	{
		throw utils::ParserException(parser.coordinate(), std::string("could not parse variable value (") + e.what() + ")");
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value &Value::referenceFromSAS(utils::Parser<> &parser, const Variable &variable)
{
	const auto valueID = parser.parse<int>();

	if (valueID == -1)
		return Value::Any;

	if (valueID < 0 || static_cast<size_t>(valueID) >= variable.values().size())
		throw utils::ParserException(parser.coordinate(), "value index out of range (variable " + variable.name() + ", index " + std::to_string(valueID) + ")");

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

void Value::printAsASPPredicate(utils::LogStream &outputStream) const
{
	// TODO: do not compare by value
	if (*this == Value::None)
	{
		outputStream << utils::Keyword("value") << "(" << utils::Reserved("none") << ")";
		return;
	}

	outputStream << utils::Keyword("value") << "(" << utils::String(m_name) << ", "
		<< (m_sign == Sign::Positive ? utils::Boolean("true") : utils::Boolean("false")) << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Value::printAsSAS(utils::LogStream &outputStream) const
{
	if (m_sign == Value::Sign::Positive)
		outputStream << "Atom ";
	else
		outputStream << "NegatedAtom ";

	outputStream << m_name;

	if (!m_hasArguments)
		outputStream << "()";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool operator ==(const Value &value1, const Value &value2)
{
	return value1.sign() == value2.sign()
		&& value1.name() == value2.name();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
