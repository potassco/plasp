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

Description::Description(std::istream &istream)
:	m_parser(istream),
	m_context(m_parser)
{
	m_parser.setCaseSensitive(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromStream(std::istream &istream)
{
	Description description(istream);

	while (true)
	{
		description.m_context.parser.skipWhiteSpace();

		if (description.m_context.parser.atEndOfFile())
			break;

		description.parseContent();
	}

	return description;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Description Description::fromFile(const boost::filesystem::path &path)
{
	if (!boost::filesystem::is_regular_file(path))
		throw std::runtime_error("File does not exist: \"" + path.string() + "\"");

	std::ifstream fileStream(path.string(), std::ios::in);

	return Description::fromStream(fileStream);
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
	std::cout << "Parsing file content" << std::endl;

	m_context.parser.expect<std::string>("(");
	m_context.parser.expect<std::string>("define");
	parseSection();
	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseSection()
{
	// Parse domain/problem identifier
	m_context.parser.expect<std::string>("(");

	const auto sectionIdentifier = m_context.parser.parse<std::string>();

	std::cout << "Parsing section " << sectionIdentifier << std::endl;

	if (sectionIdentifier == "domain")
		m_domain = std::make_unique<Domain>(Domain::fromPDDL(m_context));
	else if (sectionIdentifier == "problem")
		m_problem = std::make_unique<Problem>(Problem::fromPDDL(m_context));
	else
		throw utils::ParserException(m_context.parser, "Unknown PDDL section \"" + sectionIdentifier + "\"");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
