#include <plasp/sas/Predicate.h>

#include <iostream>
#include <limits>
#include <sstream>

#include <plasp/utils/Formatting.h>
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

Predicate Predicate::fromSAS(utils::Parser<> &parser)
{
	Predicate predicate;

	try
	{
		parser.skipLine();

		predicate.m_name = parser.parse<std::string>();

		while (true)
		{
			// Parse arguments until reaching newline
			// TODO: reimplement
			/*parser.skipWhiteSpace(
				[&](const auto character)
				{
					return character != '\n' && std::isspace(character);
				});*/

			if (parser.currentCharacter() == '\n')
				break;

			const auto value = parser.parse<std::string>();
			predicate.m_arguments.emplace_back(std::move(value));
		}
	}
	catch (const std::exception &e)
	{
		throw utils::ParserException(parser.coordinate(), "could not parse operator predicate");
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

void Predicate::printAsSAS(utils::LogStream &outputStream) const
{
	if (m_arguments.empty())
	{
		outputStream << m_name;
		return;
	}

	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		if (i > 0)
			outputStream << " ";

		outputStream << m_arguments[i];
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////

void Predicate::printAsASP(utils::LogStream &outputStream) const
{
	if (m_arguments.empty())
	{
		outputStream << utils::String(m_name);
		return;
	}

	outputStream << "(" << utils::String(m_name);

	for (size_t i = 0; i < m_arguments.size(); i++)
		outputStream << ", " << utils::String(m_arguments[i]);

	outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
