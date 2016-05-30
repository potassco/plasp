#include <plasp/pddl/Domain.h>

#include <algorithm>

#include <plasp/pddl/Identifier.h>
#include <plasp/utils/ParserException.h>

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

	domain.computeDerivedRequirements();

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

	if (m_requirements.empty())
		throw utils::ParserException(parser.row(), parser.column(), "Requirements section does not contain any requirements");

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::computeDerivedRequirements()
{
	const auto hasRequirement =
		[&](const auto requirement)
		{
			const auto match = std::find(m_requirements.cbegin(), m_requirements.cend(), requirement);

			return match != m_requirements.cend();
		};

	const auto addRequirementUnique =
		[&](const auto requirement)
		{
			if (hasRequirement(requirement))
				return;

			m_requirements.push_back(requirement);
		};

	// If no requirements are specified, assume STRIPS
	if (m_requirements.empty())
		addRequirementUnique(Requirement::Type::STRIPS);

	if (hasRequirement(Requirement::Type::ADL))
	{
		addRequirementUnique(Requirement::Type::STRIPS);
		addRequirementUnique(Requirement::Type::Typing);
		addRequirementUnique(Requirement::Type::NegativePreconditions);
		addRequirementUnique(Requirement::Type::DisjunctivePreconditions);
		addRequirementUnique(Requirement::Type::Equality);
		addRequirementUnique(Requirement::Type::QuantifiedPreconditions);
		addRequirementUnique(Requirement::Type::ConditionalEffects);
	}

	if (hasRequirement(Requirement::Type::QuantifiedPreconditions))
	{
		addRequirementUnique(Requirement::Type::ExistentialPreconditions);
		addRequirementUnique(Requirement::Type::UniversalPreconditions);
	}

	if (hasRequirement(Requirement::Type::Fluents))
	{
		addRequirementUnique(Requirement::Type::NumericFluents);
		addRequirementUnique(Requirement::Type::ObjectFluents);
	}

	if (hasRequirement(Requirement::Type::TimedInitialLiterals))
		addRequirementUnique(Requirement::Type::DurativeActions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
