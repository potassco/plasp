#include <plasp/pddl/Problem.h>

#include <algorithm>

#include <plasp/pddl/Domain.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/Constant.h>
#include <plasp/utils/IO.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Problem
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Problem::Problem(Context &context, Domain &domain)
:	m_context(context),
	m_domain(domain),
	m_isDeclared{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::readPDDL()
{
	m_name = m_context.parser.parseIdentifier(isIdentifier);

	std::cout << "Parsing problem " << m_name << std::endl;

	m_context.parser.expect<std::string>(")");

	while (true)
	{
		m_context.parser.skipWhiteSpace();

		if (m_context.parser.currentCharacter() == ')')
			break;

		parseSection();
	}

	computeDerivedRequirements();

	m_isDeclared = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Problem::isDeclared() const
{
	return m_isDeclared;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Domain &Problem::domain()
{
	return m_domain;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Domain &Problem::domain() const
{
	return m_domain;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Problem::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Requirements &Problem::requirements() const
{
	return m_requirements;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

expressions::Constants &Problem::objects()
{
	return m_objects;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const expressions::Constants &Problem::objects() const
{
	return m_objects;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parseSection()
{
	m_context.parser.expect<std::string>("(");
	m_context.parser.expect<std::string>(":");

	const auto sectionIdentifier = m_context.parser.parseIdentifier(isIdentifier);

	const auto skipSection =
		[&]()
		{
			std::cout << "Skipping section " << sectionIdentifier << std::endl;

			size_t openParentheses = 1;

			while (true)
			{
				const auto character = m_context.parser.currentCharacter();
				m_context.parser.advance();

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

	// TODO: check order of the sections
	if (sectionIdentifier == "domain")
		parseDomainSection();
	else if (sectionIdentifier == "requirements")
		parseRequirementSection();
	else if (sectionIdentifier == "objects")
		parseObjectSection();
	else if (sectionIdentifier == "init")
		skipSection();
	else if (sectionIdentifier == "goal")
		skipSection();
	else if (sectionIdentifier == "constraints")
		skipSection();
	else if (sectionIdentifier == "metric")
		skipSection();
	else if (sectionIdentifier == "length")
		skipSection();
	else
		throw utils::ParserException(m_context.parser, "Unknown problem section \"" + sectionIdentifier + "\"");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parseDomainSection()
{
	m_context.parser.skipWhiteSpace();

	const auto domainName = m_context.parser.parseIdentifier(isIdentifier);

	if (m_domain.isDeclared() && m_domain.name() != domainName)
		throw utils::ParserException(m_context.parser, "Domains do not match (\"" + m_domain.name() + "\" and \"" + domainName + "\")");

	if (!m_domain.isDeclared())
		m_domain.setName(domainName);

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parseRequirementSection()
{
	m_context.parser.skipWhiteSpace();

	while (m_context.parser.currentCharacter() != ')')
	{
		m_context.parser.expect<std::string>(":");

		m_requirements.emplace_back(Requirement::parse(m_context));

		m_context.parser.skipWhiteSpace();
	}

	// TODO: Do this check only once the domain is parsed
	// If no requirements are specified, assume STRIPS
	if (m_requirements.empty())
		m_requirements.emplace_back(Requirement::Type::STRIPS);

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Problem::hasRequirement(Requirement::Type requirementType) const
{
	const auto match = std::find_if(m_requirements.cbegin(), m_requirements.cend(),
		[&](const auto &requirement)
		{
			return requirement.type() == requirementType;
		});

	return match != m_requirements.cend();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::computeDerivedRequirements()
{
	const auto addRequirementUnique =
		[&](const auto requirement)
		{
			if (hasRequirement(requirement))
				return;

			m_requirements.push_back(Requirement(requirement));
		};

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

void Problem::parseObjectSection()
{
	m_context.parser.skipWhiteSpace();

	// Store constants
	while (m_context.parser.currentCharacter() != ')')
	{
		expressions::Constant::parseTypedDeclaration(m_context, *this);

		m_context.parser.skipWhiteSpace();
	}

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::checkConsistency()
{
	// Verify that all objects have types
	if (hasRequirement(Requirement::Type::Typing) || m_domain.hasRequirement(Requirement::Type::Typing))
		std::for_each(m_objects.cbegin(), m_objects.cend(),
			[&](const auto &constant)
			{
				if (constant->type() == nullptr)
					throw ConsistencyException("Object \"" + constant->name() + "\" has no type");
			});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
