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
	m_parser.setCaseSensitive(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromStream(std::istream &istream)
{
	Description description;

	description.m_parser.readStream("std::cin", istream);

	description.parseContent();
	description.checkConsistency();

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromFile(const std::string &path)
{
	Description description;

	description.m_parser.readFile(path);

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
	m_domain->readPDDL();

	if (m_problemPosition != -1)
	{
		m_context.parser.seek(m_problemPosition);
		m_problem->readPDDL();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::findSections()
{
	while (true)
	{
		m_context.parser.skipWhiteSpace();

		if (m_context.parser.atEndOfStream())
			return;

		const auto position = m_context.parser.position();

		m_context.parser.expect<std::string>("(");
		m_context.parser.expect<std::string>("define");
		m_context.parser.expect<std::string>("(");

		if (m_context.parser.probe<std::string>("domain"))
		{
			if (m_domainPosition != -1)
				throw utils::ParserException(m_context.parser, "PDDL description may not contain two domains");

			m_domainPosition = position;
		}
		else if (m_context.parser.probe<std::string>("problem"))
		{
			if (m_problemPosition != -1)
				throw utils::ParserException(m_context.parser, "PDDL description may currently not contain two problems");

			m_problemPosition = position;
		}
		else
		{
			const auto sectionIdentifier = m_context.parser.parse<std::string>();
			throw utils::ParserException(m_context.parser, "Unknown PDDL section \"" + sectionIdentifier + "\"");
		}

		skipSection(m_context.parser);
		skipSection(m_context.parser);
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
