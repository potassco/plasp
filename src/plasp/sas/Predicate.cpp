#include <plasp/sas/Predicate.h>

#include <iostream>
#include <limits>
#include <sstream>

#include <plasp/utils/Parsing.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Predicate Predicate::fromSAS(std::istream &istream)
{
	Predicate predicate;

	try
	{
		istream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// TODO: Inefficient, reimplement in one pass
		std::string line;
		std::getline(istream, line);

		std::stringstream lineStream(line);

		predicate.m_name = utils::parse<std::string>(lineStream);

		while (lineStream.peek() == ' ')
			lineStream.ignore(1);

		for (std::string argument; std::getline(lineStream, argument, ' ');)
			predicate.m_arguments.push_back(std::move(argument));
	}
	catch (const std::exception &e)
	{
		throw utils::ParserException("Could not parse operator predicate");
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
