#include <plasp/pddl/Problem.h>

#include <algorithm>

#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
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
	m_domainPosition{-1},
	m_requirementsPosition{-1},
	m_objectsPosition{-1},
	m_initialStatePosition{-1},
	m_goalPosition{-1}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::findSections()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>("define");
	parser.expect<std::string>("(");
	parser.expect<std::string>("problem");

	m_name = parser.parseIdentifier(isIdentifier);

	parser.expect<std::string>(")");

	const auto setSectionPosition =
	[&](const std::string &sectionName, auto &sectionPosition, const auto value, bool unique = false)
	{
		if (unique && sectionPosition != -1)
		{
			parser.seek(value);
			throw utils::ParserException(parser, "Only one “:" + sectionName + "” section allowed");
		}

		sectionPosition = value;
	};

	parser.skipWhiteSpace();

	while (parser.currentCharacter() != ')')
	{
		const auto position = parser.position();

		parser.expect<std::string>("(");
		parser.expect<std::string>(":");

		const auto sectionIdentifierPosition = parser.position();

		// TODO: check order of the sections
		if (parser.probeIdentifier("domain", isIdentifier))
			setSectionPosition("domain", m_domainPosition, position, true);
		else if (parser.probeIdentifier("requirements", isIdentifier))
			setSectionPosition("requirements", m_requirementsPosition, position, true);
		else if (parser.probeIdentifier("objects", isIdentifier))
			setSectionPosition("objects", m_objectsPosition, position, true);
		else if (parser.probeIdentifier("init", isIdentifier))
			setSectionPosition("init", m_initialStatePosition, position, true);
		else if (parser.probeIdentifier("goal", isIdentifier))
			setSectionPosition("goal", m_goalPosition, position, true);
		else if (parser.probeIdentifier("constraints", isIdentifier)
			|| parser.probeIdentifier("metric", isIdentifier)
			|| parser.probeIdentifier("length", isIdentifier))
		{
			parser.seek(sectionIdentifierPosition);

			const auto sectionIdentifier = parser.parseIdentifier(isIdentifier);

			m_context.logger.logWarning(parser, "Section type “" + sectionIdentifier + "” currently unsupported");

			parser.seek(sectionIdentifierPosition);
		}
		else
		{
			const auto sectionIdentifier = parser.parseIdentifier(isIdentifier);

			parser.seek(position);
			throw utils::ParserException(m_context.parser, "Unknown problem section “" + sectionIdentifier + "”");
		}

		// Skip section for now and parse it later
		skipSection(parser);

		parser.skipWhiteSpace();
	}

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parse()
{
	auto &parser = m_context.parser;

	if (m_domainPosition == -1)
		throw ConsistencyException("Problem description does not specify the corresponding domain");

	parser.seek(m_domainPosition);
	parseDomainSection();

	if (m_requirementsPosition != -1)
	{
		parser.seek(m_requirementsPosition);
		parseRequirementSection();
	}

	if (m_objectsPosition != -1)
	{
		parser.seek(m_objectsPosition);
		parseObjectSection();
	}

	if (m_initialStatePosition == -1)
		throw ConsistencyException("Problem description does not specify an initial state");

	parser.seek(m_initialStatePosition);
	parseInitialStateSection();

	if (m_goalPosition == -1)
		throw ConsistencyException("Problem description does not specify a goal");

	parser.seek(m_goalPosition);
	parseGoalSection();
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

void Problem::parseDomainSection()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");
	parser.expect<std::string>("domain");

	parser.skipWhiteSpace();

	const auto domainName = parser.parseIdentifier(isIdentifier);

	if (m_domain.name() != domainName)
		throw utils::ParserException(parser, "Domains do not match (“" + m_domain.name() + "” and “" + domainName + "”)");

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parseRequirementSection()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");
	parser.expect<std::string>("requirements");

	parser.skipWhiteSpace();

	while (parser.currentCharacter() != ')')
	{
		parser.expect<std::string>(":");

		m_requirements.emplace_back(Requirement::parse(m_context));

		parser.skipWhiteSpace();
	}

	// TODO: do this check only once the domain is parsed
	// If no requirements are specified, assume STRIPS
	if (m_requirements.empty())
		m_requirements.emplace_back(Requirement::Type::STRIPS);

	parser.expect<std::string>(")");
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

	throw ConsistencyException("Requirement “" + Requirement(requirementType).toPDDL() + "” used but never declared");
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
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");
	parser.expect<std::string>("objects");

	parser.skipWhiteSpace();

	// Store constants
	expressions::Constant::parseTypedDeclarations(m_context, *this);

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parseInitialStateSection()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");
	parser.expect<std::string>("init");

	// TODO: remove workaround
	expressions::Variables noParameters;
	ExpressionContext expressionContext(m_domain, this, noParameters);

	m_initialState = InitialState::parseDeclaration(m_context, expressionContext);

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parseGoalSection()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");
	parser.expect<std::string>("goal");

	// TODO: remove workaround
	expressions::Variables noParameters;
	ExpressionContext expressionContext(m_domain, this, noParameters);

	m_goal = parsePreconditionExpression(m_context, expressionContext);

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

InitialState &Problem::initialState()
{
	BOOST_ASSERT(m_initialState);

	return *m_initialState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const InitialState &Problem::initialState() const
{
	BOOST_ASSERT(m_initialState);

	return *m_initialState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Expression &Problem::goal() const
{
	BOOST_ASSERT(m_goal);

	return *m_goal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::checkConsistency()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
