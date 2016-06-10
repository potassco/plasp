#include <plasp/pddl/Description.h>

#include <fstream>
#include <sstream>

#include <boost/filesystem.hpp>

#include <plasp/pddl/IO.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Description::Description()
:	m_context(m_parser),
	m_domainPosition{-1},
	m_domain{std::make_unique<Domain>(Domain(m_context))},
	m_problemPosition{-1},
	m_problem{std::make_unique<Problem>(Problem(m_context, *m_domain))}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromParser(utils::Parser &&parser)
{
	Description description;

	parser.setCaseSensitive(false);
	parser.removeComments(";", "\n", false);

	description.m_parser = std::move(parser);

	description.parseContent();
	description.checkConsistency();

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromStream(std::istream &istream)
{
	Description description;

	description.m_parser.readStream("std::cin", istream);

	description.m_parser.setCaseSensitive(false);
	description.m_parser.removeComments(";", "\n", false);

	description.parseContent();
	description.checkConsistency();

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromFile(const std::string &path)
{
	Description description;

	description.m_parser.readFile(path);

	description.m_parser.setCaseSensitive(false);
	description.m_parser.removeComments(";", "\n", false);

	description.parseContent();
	description.checkConsistency();

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromFiles(const std::vector<std::string> &paths)
{
	BOOST_ASSERT(!paths.empty());

	Description description;

	std::for_each(paths.cbegin(), paths.cend(),
		[&](const auto &path)
		{
			description.m_parser.readFile(path);
		});

	description.m_parser.setCaseSensitive(false);
	description.m_parser.removeComments(";", "\n", false);

	description.parseContent();
	description.checkConsistency();

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Domain &Description::domain() const
{
	BOOST_ASSERT(m_domain);

	return *m_domain;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Description::containsProblem() const
{
	return m_problem.get() != nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Problem &Description::problem() const
{
	BOOST_ASSERT(m_problem);

	return *m_problem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseContent()
{
	// First, determine the locations of domain and problem
	findSections();

	if (m_domainPosition == -1)
		throw ConsistencyException("No PDDL domain specified");

	m_context.parser.seek(m_domainPosition);
	m_domain->parse();

	if (m_problemPosition != -1)
	{
		m_context.parser.seek(m_problemPosition);
		m_problem->parse();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::findSections()
{
	auto &parser = m_context.parser;

	parser.skipWhiteSpace();

	while (!parser.atEndOfStream())
	{
		const auto position = parser.position();

		parser.expect<std::string>("(");
		parser.expect<std::string>("define");
		parser.expect<std::string>("(");

		if (parser.probe<std::string>("domain"))
		{
			if (m_domainPosition != -1)
				throw utils::ParserException(parser, "PDDL description may not contain two domains");

			m_domainPosition = position;

			parser.seek(position);
			m_domain->findSections();
		}
		else if (m_context.parser.probe<std::string>("problem"))
		{
			if (m_problemPosition != -1)
				throw utils::ParserException(parser, "PDDL description may currently not contain two problems");

			m_problemPosition = position;

			parser.seek(position);
			m_problem->findSections();
		}
		else
		{
			const auto sectionIdentifier = parser.parse<std::string>();
			throw utils::ParserException(parser, "Unknown PDDL section \"" + sectionIdentifier + "\"");
		}

		m_context.parser.skipWhiteSpace();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::checkConsistency()
{
	m_domain->checkConsistency();
	m_problem->checkConsistency();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
