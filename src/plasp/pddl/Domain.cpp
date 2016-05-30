#include <plasp/pddl/Domain.h>

#include <plasp/pddl/Identifier.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Domain
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Domain Domain::fromPDDL(utils::Parser &parser)
{
	Domain domain;

	domain.m_name = parser.parseIdentifier(isIdentifier);

	std::cout << "Parsing domain " << domain.m_name << std::endl;

	parser.expect<std::string>(")");

	while (true)
	{
		parser.skipWhiteSpace();

		if (parser.currentCharacter() == ')')
			break;

		domain.parseSection(parser);
	}

	return domain;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Domain::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Requirement::Types &Domain::requirements() const
{
	return m_requirements;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseSection(utils::Parser &parser)
{
	parser.expect<std::string>("(:");

	const auto sectionIdentifier = parser.parseIdentifier(isIdentifier);

	const auto skipSection =
		[&]()
		{
			std::cout << "Skipping section " << sectionIdentifier << std::endl;

			size_t openParentheses = 1;

			while (true)
			{
				const auto character = parser.currentCharacter();
				parser.advance();

				if (character == '(')
					openParentheses++;
				else if (character == ')')
				{
					openParentheses--;

					if (openParentheses == 0)
						return;
				}
			}
		};

	if (sectionIdentifier == "requirements")
		parseRequirementsSection(parser);
	else if (sectionIdentifier == "types")
		skipSection();
	else if (sectionIdentifier == "constants")
		skipSection();
	else if (sectionIdentifier == "predicates")
		skipSection();
	else if (sectionIdentifier == "functions")
		skipSection();
	else if (sectionIdentifier == "constraints")
		skipSection();
	else if (sectionIdentifier == "action")
		skipSection();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseRequirementsSection(utils::Parser &parser)
{
	while (true)
	{
		parser.skipWhiteSpace();

		if (parser.currentCharacter() == ')')
			break;

		if (parser.currentCharacter() == ':')
			parser.advance();

		m_requirements.emplace_back(Requirement::fromPDDL(parser));
	}

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
