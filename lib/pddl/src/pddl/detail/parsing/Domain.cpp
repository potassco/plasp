#include <pddl/detail/parsing/Domain.h>

#include <pddl/Exception.h>
#include <pddl/detail/Requirements.h>
#include <pddl/detail/parsing/Action.h>
#include <pddl/detail/parsing/ConstantDeclaration.h>
#include <pddl/detail/parsing/PredicateDeclaration.h>
#include <pddl/detail/parsing/PrimitiveTypeDeclaration.h>
#include <pddl/detail/parsing/Requirement.h>
#include <pddl/detail/parsing/Unsupported.h>
#include <pddl/detail/parsing/Utils.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Domain
//
////////////////////////////////////////////////////////////////////////////////////////////////////

DomainParser::DomainParser(Context &context)
:	m_context{context},
	m_requirementsPosition{tokenize::InvalidStreamPosition},
	m_typesPosition{tokenize::InvalidStreamPosition},
	m_constantsPosition{tokenize::InvalidStreamPosition},
	m_predicatesPosition{tokenize::InvalidStreamPosition}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::DomainPointer DomainParser::parse()
{
	auto domain = std::make_unique<ast::Domain>();

	findSections(*domain);

	auto &tokenizer = m_context.tokenizer;

	if (m_requirementsPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_requirementsPosition);
		parseRequirementSection(*domain);
	}

	if (m_typesPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_typesPosition);
		parseTypeSection(*domain);
	}

	if (m_constantsPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_constantsPosition);
		parseConstantSection(*domain);
	}

	if (m_predicatesPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_predicatesPosition);
		parsePredicateSection(*domain);
	}

	for (size_t i = 0; i < m_actionPositions.size(); i++)
		if (m_actionPositions[i] != tokenize::InvalidStreamPosition)
		{
			tokenizer.seek(m_actionPositions[i]);
			parseActionSection(*domain);
		}

	computeDerivedRequirements(*domain);

	return domain;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DomainParser::findSections(ast::Domain &domain)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>("define");
	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>("domain");

	domain.name = tokenizer.getIdentifier();

	tokenizer.expect<std::string>(")");

	const auto setSectionPosition =
		[&](const std::string &sectionName, auto &sectionPosition, const auto value, bool unique = false)
		{
			if (unique && sectionPosition != tokenize::InvalidStreamPosition)
			{
				tokenizer.seek(value);
				throw ParserException(tokenizer.location(), "only one “:" + sectionName + "” section allowed");
			}

			sectionPosition = value;
		};

	tokenizer.skipWhiteSpace();

	// Find sections
	while (tokenizer.currentCharacter() != ')')
	{
		const auto position = tokenizer.position();

		tokenizer.expect<std::string>("(");
		tokenizer.expect<std::string>(":");

		// Save the parser position of the individual sections for later parsing
		if (tokenizer.testIdentifierAndSkip("requirements"))
			setSectionPosition("requirements", m_requirementsPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("types"))
			setSectionPosition("types", m_typesPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("constants"))
			setSectionPosition("constants", m_constantsPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("predicates"))
			setSectionPosition("predicates", m_predicatesPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("action"))
		{
			m_actionPositions.emplace_back(tokenize::InvalidStreamPosition);
			setSectionPosition("action", m_actionPositions.back(), position);
		}
		else if (tokenizer.testIdentifierAndSkip("functions")
			|| tokenizer.testIdentifierAndSkip("constraints")
			|| tokenizer.testIdentifierAndSkip("durative-action")
			|| tokenizer.testIdentifierAndSkip("derived"))
		{
			throw exceptUnsupportedSection(position, m_context);
		}
		else
		{
			const auto sectionIdentifier = tokenizer.getIdentifier();

			tokenizer.seek(position);
			throw ParserException(tokenizer.location(), "unknown domain section “" + sectionIdentifier + "”");
		}

		// Skip section for now and parse it later
		skipSection(tokenizer);

		tokenizer.skipWhiteSpace();
	}

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DomainParser::parseRequirementSection(ast::Domain &domain)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("requirements");

	while (tokenizer.currentCharacter() != ')')
	{
		tokenizer.expect<std::string>(":");

		const auto requirement = parseRequirement(m_context);

		if (requirement)
			domain.requirements.emplace_back(requirement.value());

		tokenizer.skipWhiteSpace();
	}

	// TODO: do this check only once the problem is parsed
	// If no requirements are specified, assume STRIPS
	if (domain.requirements.empty())
		domain.requirements.emplace_back(ast::Requirement::STRIPS);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DomainParser::computeDerivedRequirements(ast::Domain &domain)
{
	const auto addRequirementUnique =
		[&](auto requirement)
		{
			if (hasRequirement(domain, requirement))
				return;

			domain.requirements.push_back(requirement);
		};

	if (hasRequirement(domain, ast::Requirement::ADL))
	{
		addRequirementUnique(ast::Requirement::STRIPS);
		addRequirementUnique(ast::Requirement::Typing);
		addRequirementUnique(ast::Requirement::NegativePreconditions);
		addRequirementUnique(ast::Requirement::DisjunctivePreconditions);
		addRequirementUnique(ast::Requirement::Equality);
		addRequirementUnique(ast::Requirement::QuantifiedPreconditions);
		addRequirementUnique(ast::Requirement::ConditionalEffects);
	}

	if (hasRequirement(domain, ast::Requirement::QuantifiedPreconditions))
	{
		addRequirementUnique(ast::Requirement::ExistentialPreconditions);
		addRequirementUnique(ast::Requirement::UniversalPreconditions);
	}

	if (hasRequirement(domain, ast::Requirement::Fluents))
	{
		addRequirementUnique(ast::Requirement::NumericFluents);
		addRequirementUnique(ast::Requirement::ObjectFluents);
	}

	if (hasRequirement(domain, ast::Requirement::TimedInitialLiterals))
		addRequirementUnique(ast::Requirement::DurativeActions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DomainParser::parseTypeSection(ast::Domain &domain)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("types");

	checkRequirement(domain, ast::Requirement::Typing, m_context);

	tokenizer.skipWhiteSpace();

	// Store types and their parent types
	while (tokenizer.currentCharacter() != ')')
	{
		if (tokenizer.currentCharacter() == '(')
			throw ParserException(tokenizer.location(), "only primitive types are allowed in type section");

		parseAndAddPrimitiveTypeDeclarations(m_context, domain);

		tokenizer.skipWhiteSpace();
	}

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DomainParser::parseConstantSection(ast::Domain &domain)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("constants");

	// Store constants
	parseAndAddConstantDeclarations(m_context, domain);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DomainParser::parsePredicateSection(ast::Domain &domain)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("predicates");

	tokenizer.skipWhiteSpace();

	// Store predicates
	parseAndAddPredicateDeclarations(m_context, domain);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void DomainParser::parseActionSection(ast::Domain &domain)
{
	domain.actions.emplace_back(ActionParser(m_context, domain).parse());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
