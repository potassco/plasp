#include <plasp/pddl/Problem.h>

#include <algorithm>

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/IO.h>
#include <plasp/pddl/expressions/Constant.h>

#include <tokenize/TokenizerException.h>

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
	m_domainPosition{tokenize::InvalidStreamPosition},
	m_requirementsPosition{tokenize::InvalidStreamPosition},
	m_objectsPosition{tokenize::InvalidStreamPosition},
	m_initialStatePosition{tokenize::InvalidStreamPosition},
	m_goalPosition{tokenize::InvalidStreamPosition}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::findSections()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>("define");
	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>("problem");

	m_name = tokenizer.getIdentifier();

	tokenizer.expect<std::string>(")");

	const auto setSectionPosition =
	[&](const std::string &sectionName, auto &sectionPosition, const auto value, bool unique = false)
	{
		if (unique && sectionPosition != tokenize::InvalidStreamPosition)
		{
			tokenizer.seek(value);
			throw tokenize::TokenizerException(tokenizer, "only one “:" + sectionName + "” section allowed");
		}

		sectionPosition = value;
	};

	tokenizer.skipWhiteSpace();

	while (tokenizer.currentCharacter() != ')')
	{
		const auto position = tokenizer.position();

		tokenizer.expect<std::string>("(");
		tokenizer.expect<std::string>(":");

		const auto sectionIdentifierPosition = tokenizer.position();

		// TODO: check order of the sections
		if (tokenizer.testIdentifierAndSkip("domain"))
			setSectionPosition("domain", m_domainPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("requirements"))
			setSectionPosition("requirements", m_requirementsPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("objects"))
			setSectionPosition("objects", m_objectsPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("init"))
			setSectionPosition("init", m_initialStatePosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("goal"))
			setSectionPosition("goal", m_goalPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("constraints")
			|| tokenizer.testIdentifierAndSkip("metric")
			|| tokenizer.testIdentifierAndSkip("length"))
		{
			tokenizer.seek(sectionIdentifierPosition);

			const auto sectionIdentifier = tokenizer.getIdentifier();

			m_context.logger.log(output::Priority::Warning, tokenizer, "section type “" + sectionIdentifier + "” currently unsupported");

			tokenizer.seek(sectionIdentifierPosition);
		}
		else
		{
			const auto sectionIdentifier = tokenizer.getIdentifier();

			tokenizer.seek(position);
			throw tokenize::TokenizerException(tokenizer, "unknown problem section “" + sectionIdentifier + "”");
		}

		// Skip section for now and parse it later
		skipSection(tokenizer);

		tokenizer.skipWhiteSpace();
	}

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parse()
{
	auto &tokenizer = m_context.tokenizer;

	if (m_domainPosition == tokenize::InvalidStreamPosition)
		throw ConsistencyException("problem description does not specify the corresponding domain");

	tokenizer.seek(m_domainPosition);
	parseDomainSection();

	if (m_requirementsPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_requirementsPosition);
		parseRequirementSection();
	}

	if (m_objectsPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_objectsPosition);
		parseObjectSection();
	}

	if (m_initialStatePosition == tokenize::InvalidStreamPosition)
		throw ConsistencyException("problem description does not specify an initial state");

	tokenizer.seek(m_initialStatePosition);
	parseInitialStateSection();

	if (m_goalPosition == tokenize::InvalidStreamPosition)
		throw ConsistencyException("problem description does not specify a goal");

	tokenizer.seek(m_goalPosition);
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
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("domain");

	tokenizer.skipWhiteSpace();

	const auto domainName = tokenizer.getIdentifier();

	if (m_domain.name() != domainName)
		throw tokenize::TokenizerException(tokenizer, "domains do not match (“" + m_domain.name() + "” and “" + domainName + "”)");

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parseRequirementSection()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("requirements");

	tokenizer.skipWhiteSpace();

	while (tokenizer.currentCharacter() != ')')
	{
		tokenizer.expect<std::string>(":");

		m_requirements.emplace_back(Requirement::parse(m_context));

		tokenizer.skipWhiteSpace();
	}

	// TODO: do this check only once the domain is parsed
	// If no requirements are specified, assume STRIPS
	if (m_requirements.empty())
		m_requirements.emplace_back(Requirement::Type::STRIPS);

	tokenizer.expect<std::string>(")");
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

void Problem::checkRequirement(Requirement::Type requirementType)
{
	if (hasRequirement(requirementType))
		return;

	m_context.logger.log(output::Priority::Warning, m_context.tokenizer, "requirement “" + Requirement(requirementType).toPDDL() + "” used but never declared");

	m_requirements.push_back(requirementType);
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
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("objects");

	tokenizer.skipWhiteSpace();

	// Store constants
	expressions::Constant::parseTypedDeclarations(m_context, *this);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parseInitialStateSection()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("init");

	ExpressionContext expressionContext(m_domain, this);

	m_initialState = InitialState::parseDeclaration(m_context, expressionContext);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Problem::parseGoalSection()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("goal");

	ExpressionContext expressionContext(m_domain, this);

	m_goal = parsePreconditionExpression(m_context, expressionContext);

	tokenizer.expect<std::string>(")");
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

void Problem::normalize()
{
	BOOST_ASSERT(m_goal);

	// TODO: normalize objects and initial state

	m_goal = m_goal->normalized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
