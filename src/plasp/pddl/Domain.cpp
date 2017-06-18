#include <plasp/pddl/Domain.h>

#include <algorithm>

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/IO.h>
#include <plasp/pddl/expressions/Constant.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/PrimitiveType.h>
#include <plasp/pddl/expressions/Variable.h>

#include <tokenize/TokenizerException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Domain
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Domain::Domain(Context &context)
:	m_context(context),
	m_requirementsPosition{tokenize::InvalidStreamPosition},
	m_typesPosition{tokenize::InvalidStreamPosition},
	m_constantsPosition{tokenize::InvalidStreamPosition},
	m_predicatesPosition{tokenize::InvalidStreamPosition}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::findSections()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>("define");
	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>("domain");

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

	// Find sections
	while (tokenizer.currentCharacter() != ')')
	{
		const auto position = tokenizer.position();

		tokenizer.expect<std::string>("(");
		tokenizer.expect<std::string>(":");

		const auto sectionIdentifierPosition = tokenizer.position();

		// Save the tokenizer position of the individual sections for later parsing
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
			m_actionPositions.emplace_back(-1);
			setSectionPosition("action", m_actionPositions.back(), position);
		}
		else if (tokenizer.testIdentifierAndSkip("functions")
			|| tokenizer.testIdentifierAndSkip("constraints")
			|| tokenizer.testIdentifierAndSkip("durative-action")
			|| tokenizer.testIdentifierAndSkip("derived"))
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
			throw tokenize::TokenizerException(tokenizer, "unknown domain section “" + sectionIdentifier + "”");
		}

		// Skip section for now and parse it later
		skipSection(tokenizer);

		tokenizer.skipWhiteSpace();
	}

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parse()
{
	auto &tokenizer = m_context.tokenizer;

	if (m_requirementsPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_requirementsPosition);
		parseRequirementSection();
	}

	if (m_typesPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_typesPosition);
		parseTypeSection();
	}

	if (m_constantsPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_constantsPosition);
		parseConstantSection();
	}

	if (m_predicatesPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_predicatesPosition);
		parsePredicateSection();
	}

	for (size_t i = 0; i < m_actionPositions.size(); i++)
		if (m_actionPositions[i] != tokenize::InvalidStreamPosition)
		{
			tokenizer.seek(m_actionPositions[i]);
			parseActionSection();
		}

	computeDerivedRequirements();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::setName(std::string name)
{
	m_name = name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Domain::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Requirements &Domain::requirements() const
{
	return m_requirements;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

expressions::PrimitiveTypes &Domain::types()
{
	return m_types;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const expressions::PrimitiveTypes &Domain::types() const
{
	return m_types;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

expressions::Constants &Domain::constants()
{
	return m_constants;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const expressions::Constants &Domain::constants() const
{
	return m_constants;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

expressions::PredicateDeclarations &Domain::predicates()
{
	return m_predicates;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const expressions::PredicateDeclarations &Domain::predicates() const
{
	return m_predicates;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::unique_ptr<Action>> &Domain::actions()
{
	return m_actions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<std::unique_ptr<Action>> &Domain::actions() const
{
	return m_actions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

expressions::DerivedPredicates &Domain::derivedPredicates()
{
	return m_derivedPredicates;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const expressions::DerivedPredicates &Domain::derivedPredicates() const
{
	return m_derivedPredicates;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseRequirementSection()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("requirements");

	while (tokenizer.currentCharacter() != ')')
	{
		tokenizer.expect<std::string>(":");

		m_requirements.emplace_back(Requirement::parse(m_context));

		tokenizer.skipWhiteSpace();
	}

	// TODO: do this check only once the problem is parsed
	// If no requirements are specified, assume STRIPS
	if (m_requirements.empty())
		m_requirements.emplace_back(Requirement::Type::STRIPS);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Domain::hasRequirement(Requirement::Type requirementType) const
{
	const auto match = std::find_if(m_requirements.cbegin(), m_requirements.cend(),
		[&](const auto &requirement)
		{
			return requirement.type() == requirementType;
		});

	return match != m_requirements.cend();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::checkRequirement(Requirement::Type requirementType)
{
	if (hasRequirement(requirementType))
		return;

	m_context.logger.log(output::Priority::Warning, m_context.tokenizer, "requirement “" + Requirement(requirementType).toPDDL() + "” used but never declared");

	m_requirements.push_back(requirementType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::computeDerivedRequirements()
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

void Domain::parseTypeSection()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("types");

	checkRequirement(Requirement::Type::Typing);

	tokenizer.skipWhiteSpace();

	// Store types and their parent types
	while (tokenizer.currentCharacter() != ')')
	{
		if (tokenizer.currentCharacter() == '(')
			throw tokenize::TokenizerException(tokenizer, "only primitive types are allowed in type section");

		expressions::PrimitiveType::parseTypedDeclaration(m_context, *this);

		tokenizer.skipWhiteSpace();
	}

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseConstantSection()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("constants");

	// Store constants
	expressions::Constant::parseTypedDeclarations(m_context, *this);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parsePredicateSection()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("predicates");

	tokenizer.skipWhiteSpace();

	// Store predicates and their arguments
	while (tokenizer.currentCharacter() != ')')
	{
		expressions::PredicateDeclaration::parse(m_context, *this);

		tokenizer.skipWhiteSpace();
	}

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseActionSection()
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("action");

	Action::parseDeclaration(m_context, *this);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::checkConsistency()
{
	// Verify that constants are unique
	// Verify that all primitive types are unique
	// Check for case-sensitivity issues
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::normalize()
{
	std::for_each(m_predicates.begin(), m_predicates.end(),
		[](auto &predicate)
		{
			predicate->normalizeParameterNames();
		});

	std::for_each(m_actions.begin(), m_actions.end(),
		[&](auto &action)
		{
			action->normalize(m_derivedPredicates);
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
