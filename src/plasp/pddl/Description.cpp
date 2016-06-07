#include <plasp/pddl/Description.h>

#include <fstream>
#include <sstream>

#include <boost/filesystem.hpp>

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
	m_domain{std::make_unique<Domain>(Domain(m_context))},
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

void Description::parseContent()
{
	while (true)
	{
		m_context.parser.skipWhiteSpace();

		if (m_context.parser.atEndOfStream())
			return;

		m_context.parser.expect<std::string>("(");
		m_context.parser.expect<std::string>("define");
		parseSection();
		m_context.parser.expect<std::string>(")");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseSection()
{
	// Parse domain/problem identifier
	m_context.parser.expect<std::string>("(");

	const auto sectionIdentifier = m_context.parser.parse<std::string>();

	if (sectionIdentifier == "domain")
	{
		BOOST_ASSERT(m_domain);

		if (m_domain->isDeclared())
			throw utils::ParserException(m_context.parser, "PDDL description may not contain two domains");

		m_domain->readPDDL();
	}
	else if (sectionIdentifier == "problem")
	{
		BOOST_ASSERT(m_problem);

		if (m_problem->isDeclared())
			throw utils::ParserException(m_context.parser, "PDDL description may currently not contain two problems");

		m_problem->readPDDL();
	}
	else
		throw utils::ParserException(m_context.parser, "Unknown PDDL section \"" + sectionIdentifier + "\"");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::checkConsistency()
{
	if (!m_domain->isDeclared())
		throw ConsistencyException("No PDDL domain specified");

	if (m_problem->hasRequirement(Requirement::Type::Typing)
		&& !m_domain->hasRequirement(Requirement::Type::Typing))
	{
		throw ConsistencyException("PDDL problems may not add the \"typing\" requirement");
	}

	m_domain->checkConsistency();
	m_problem->checkConsistency();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
