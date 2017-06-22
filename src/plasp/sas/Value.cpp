#include <plasp/sas/Value.h>

#include <iostream>

#include <tokenize/TokenizerException.h>

#include <colorlog/Formatting.h>

#include <plasp/sas/Variable.h>

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

Value Value::fromSAS(tokenize::Tokenizer<> &tokenizer)
{
	const auto sasSign = tokenizer.get<std::string>();

	if (sasSign == "<none")
	{
		tokenizer.expect<std::string>("of");
		tokenizer.expect<std::string>("those>");

		// TODO: do not return a copy of Value::None
		return Value::None;
	}

	Value value;

	if (sasSign == "Atom")
		value.m_sign = Value::Sign::Positive;
	else if (sasSign == "NegatedAtom")
		value.m_sign = Value::Sign::Negative;
	else
		throw tokenize::TokenizerException(tokenizer.location(), "invalid value sign “" + sasSign + "”");

	try
	{
		tokenizer.skipWhiteSpace();
		value.m_name = tokenizer.getLine();

		// Remove trailing ()
		if (value.m_name.find("()") != std::string::npos)
		{
			value.m_hasArguments = false;
			value.m_name.resize(value.m_name.size() - 2);
		}
	}
	catch (const std::exception &e)
	{
		throw tokenize::TokenizerException(tokenizer.location(), std::string("could not parse variable value (") + e.what() + ")");
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Value &Value::referenceFromSAS(tokenize::Tokenizer<> &tokenizer, const Variable &variable)
{
	const auto valueID = tokenizer.get<int>();

	if (valueID == -1)
		return Value::Any;

	if (valueID < 0 || static_cast<size_t>(valueID) >= variable.values().size())
		throw tokenize::TokenizerException(tokenizer.location(), "value index out of range (variable " + variable.name() + ", index " + std::to_string(valueID) + ")");

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

void Value::printAsASPPredicate(colorlog::ColorStream &stream) const
{
	// TODO: do not compare by value
	if (*this == Value::None)
	{
		stream << colorlog::Keyword("value") << "(" << colorlog::Reserved("none") << ")";
		return;
	}

	stream << colorlog::Keyword("value") << "(" << colorlog::String(m_name.c_str()) << ", "
		<< (m_sign == Sign::Positive ? colorlog::Boolean("true") : colorlog::Boolean("false")) << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Value::printAsSAS(colorlog::ColorStream &stream) const
{
	if (m_sign == Value::Sign::Positive)
		stream << "Atom ";
	else
		stream << "NegatedAtom ";

	stream << m_name;

	if (!m_hasArguments)
		stream << "()";
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
