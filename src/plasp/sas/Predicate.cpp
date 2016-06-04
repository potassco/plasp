#include <plasp/sas/Predicate.h>

#include <iostream>
#include <limits>
#include <sstream>

#include <plasp/utils/IO.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Predicate Predicate::fromSAS(utils::Parser &parser)
{
	Predicate predicate;

	try
	{
		parser.skipLine();

		predicate.m_name = parser.parse<std::string>();

		while (true)
		{
			// Parse arguments until reaching newline
			parser.skipWhiteSpace(
				[&](const auto character)
				{
					return character != '\n' && std::isspace(character);
				});

			if (parser.currentCharacter() == '\n')
				break;

			const auto value = parser.parse<std::string>();
			predicate.m_arguments.emplace_back(std::move(value));
		}
	}
	catch (const std::exception &e)
	{
		throw utils::ParserException(parser, "Could not parse operator predicate");
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

void Predicate::printAsSAS(std::ostream &ostream) const
{
	if (m_arguments.empty())
	{
		ostream << m_name;
		return;
	}

	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		if (i > 0)
			ostream << " ";

		ostream << m_arguments[i];
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////

void Predicate::printAsASP(std::ostream &ostream) const
{
	if (m_arguments.empty())
	{
		ostream << utils::escapeASP(m_name);
		return;
	}

	ostream << utils::escapeASP(m_name) << "(";

	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		if (i > 0)
			ostream << ", ";

		ostream << utils::escapeASP(m_arguments[i]);
	}

	ostream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
