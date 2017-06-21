#include <plasp/sas/Predicate.h>

#include <iostream>
#include <limits>
#include <sstream>

#include <plasp/output/Formatting.h>

#include <tokenize/TokenizerException.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Predicate Predicate::fromSAS(tokenize::Tokenizer<> &tokenizer)
{
	Predicate predicate;

	try
	{
		tokenizer.skipLine();

		predicate.m_name = tokenizer.get<std::string>();

		while (true)
		{
			// Skip whitespace but not newlines
			tokenizer.skipBlankSpace();

			// TODO: check \r handling
			if (tokenizer.currentCharacter() == '\n')
				break;

			const auto value = tokenizer.get<std::string>();
			predicate.m_arguments.emplace_back(std::move(value));
		}
	}
	catch (const std::exception &e)
	{
		throw tokenize::TokenizerException(tokenizer.location(), "could not parse operator predicate");
	}

	return predicate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Predicate::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Predicate::Arguments &Predicate::arguments() const
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Predicate::printAsSAS(output::ColorStream &stream) const
{
	if (m_arguments.empty())
	{
		stream << m_name;
		return;
	}

	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		if (i > 0)
			stream << " ";

		stream << m_arguments[i];
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////

void Predicate::printAsASP(output::ColorStream &stream) const
{
	if (m_arguments.empty())
	{
		stream << output::String(m_name.c_str());
		return;
	}

	stream << "(" << output::String(m_name.c_str());

	for (size_t i = 0; i < m_arguments.size(); i++)
		stream << ", " << output::String(m_arguments[i].c_str());

	stream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
