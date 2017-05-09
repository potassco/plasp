#include <plasp/sas/Predicate.h>

#include <iostream>
#include <limits>
#include <sstream>

#include <plasp/output/Formatting.h>

#include <parsebase/ParserException.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Predicate Predicate::fromSAS(parsebase::Parser<> &parser)
{
	Predicate predicate;

	try
	{
		parser.skipLine();

		predicate.m_name = parser.parse<std::string>();

		while (true)
		{
			// Skip whitespace but not newlines
			parser.skipBlankSpace();

			// TODO: check \r handling
			if (parser.currentCharacter() == '\n')
				break;

			const auto value = parser.parse<std::string>();
			predicate.m_arguments.emplace_back(std::move(value));
		}
	}
	catch (const std::exception &e)
	{
		throw parsebase::ParserException(parser.location(), "could not parse operator predicate");
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
