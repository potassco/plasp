#include <plasp/pddl/Problem.h>

#include <algorithm>

#include <plasp/pddl/Domain.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/IO.h>
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
	m_context.parser.expect<std::string>("(");
	m_context.parser.expect<std::string>("define");
	m_context.parser.expect<std::string>("(");
	m_context.parser.expect<std::string>("problem");

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
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");

	// TODO: check order of the sections
	if (parser.probe<std::string>("domain"))
		parseDomainSection();
	else if (parser.probe<std::string>("requirements"))
		parseRequirementSection();
	else if (parser.probe<std::string>("objects"))
		parseObjectSection();
	else if (parser.probe<std::string>("init")
		|| parser.probe<std::string>("goal")
		|| parser.probe<std::string>("constraints")
		|| parser.probe<std::string>("metric")
		|| parser.probe<std::string>("length"))
	{
		std::cout << "Skipping section" << std::endl;
		skipSection(m_context.parser);
	}
	else
	{
		const auto sectionIdentifier = parser.parseIdentifier(isIdentifier);
		throw utils::ParserException(m_context.parser, "Unknown problem section \"" + sectionIdentifier + "\"");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parseDomainSection()
{
	m_context.parser.skipWhiteSpace();

	const auto domainName = m_context.parser.parseIdentifier(isIdentifier);

	if (m_domain.name() != domainName)
		throw utils::ParserException(m_context.parser, "Domains do not match (\"" + m_domain.name() + "\" and \"" + domainName + "\")");

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

	// TODO: do this check only once the domain is parsed
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

	if (match != m_requirements.cend())
		return true;

	return m_domain.hasRequirement(requirementType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::checkRequirement(Requirement::Type requirementType) const
{
	if (hasRequirement(requirementType))
		return;

	throw ConsistencyException("Requirement \"" + Requirement(requirementType).toPDDL() + "\" used but never declared");
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
	expressions::Constant::parseTypedDeclarations(m_context, *this);

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::checkConsistency()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
