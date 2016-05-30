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

Description Description::fromStream(std::istream &istream)
{
	Description description;

	utils::Parser parser(istream);

	while (true)
	{
		parser.skipWhiteSpace();

		if (parser.atEndOfFile())
			break;

		description.parseContent(parser);
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

void Description::parseContent(utils::Parser &parser)
{
	std::cout << "Parsing file content" << std::endl;

	parser.expect<std::string>("(define");
	parseSection(parser);
	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Description::parseSection(utils::Parser &parser)
{
	// Parse domain/problem identifier
	parser.expect<std::string>("(");

	const auto sectionIdentifier = parser.parse<std::string>();

	std::cout << "Parsing section " << sectionIdentifier << std::endl;

	if (sectionIdentifier == "domain")
		m_domain = std::make_unique<Domain>(Domain::fromPDDL(parser));
	//else if (sectionIdentifier == "problem")
	//	m_problem = std::make_unique<Problem>(Problem::fromPDDL(parser));
	else
		throw utils::ParserException(parser.row(), parser.column(), "Unknown PDDL section \"" + sectionIdentifier + "\"");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
