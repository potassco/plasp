#include <plasp/pddl/Domain.h>

#include <algorithm>

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/IO.h>
#include <plasp/pddl/expressions/Constant.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/PrimitiveType.h>
#include <plasp/pddl/expressions/Variable.h>
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

Domain::Domain(Context &context)
:	m_context(context),
	m_requirementsPosition{-1},
	m_typesPosition{-1},
	m_constantsPosition{-1},
	m_predicatesPosition{-1}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::findSections()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>("define");
	parser.expect<std::string>("(");
	parser.expect<std::string>("domain");

	m_name = m_context.parser.parseIdentifier();

	parser.expect<std::string>(")");

	const auto setSectionPosition =
		[&](const std::string &sectionName, auto &sectionPosition, const auto value, bool unique = false)
		{
			if (unique && sectionPosition != -1)
			{
				parser.seek(value);
				throw utils::ParserException(parser.coordinate(), "only one “:" + sectionName + "” section allowed");
			}

			sectionPosition = value;
		};

	parser.skipWhiteSpace();

	// Find sections
	while (parser.currentCharacter() != ')')
	{
		const auto position = parser.position();

		parser.expect<std::string>("(");
		parser.expect<std::string>(":");

		const auto sectionIdentifierPosition = parser.position();

		// Save the parser position of the individual sections for later parsing
		if (parser.testIdentifierAndSkip("requirements"))
			setSectionPosition("requirements", m_requirementsPosition, position, true);
		else if (parser.testIdentifierAndSkip("types"))
			setSectionPosition("types", m_typesPosition, position, true);
		else if (parser.testIdentifierAndSkip("constants"))
			setSectionPosition("constants", m_constantsPosition, position, true);
		else if (parser.testIdentifierAndSkip("predicates"))
			setSectionPosition("predicates", m_predicatesPosition, position, true);
		else if (parser.testIdentifierAndSkip("action"))
		{
			m_actionPositions.emplace_back(-1);
			setSectionPosition("action", m_actionPositions.back(), position);
		}
		else if (parser.testIdentifierAndSkip("functions")
			|| parser.testIdentifierAndSkip("constraints")
			|| parser.testIdentifierAndSkip("durative-action")
			|| parser.testIdentifierAndSkip("derived"))
		{
			parser.seek(sectionIdentifierPosition);

			const auto sectionIdentifier = parser.parseIdentifier();

			m_context.logger.logWarning(parser.coordinate(), "section type “" + sectionIdentifier + "” currently unsupported");

			parser.seek(sectionIdentifierPosition);
		}
		else
		{
			const auto sectionIdentifier = parser.parseIdentifier();

			parser.seek(position);
			throw utils::ParserException(parser.coordinate(), "unknown domain section “" + sectionIdentifier + "”");
		}

		// Skip section for now and parse it later
		skipSection(parser);

		parser.skipWhiteSpace();
	}

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parse()
{
	auto &parser = m_context.parser;

	if (m_requirementsPosition != -1)
	{
		parser.seek(m_requirementsPosition);
		parseRequirementSection();
	}

	if (m_typesPosition != -1)
	{
		parser.seek(m_typesPosition);
		parseTypeSection();
	}

	if (m_constantsPosition != -1)
	{
		parser.seek(m_constantsPosition);
		parseConstantSection();
	}

	if (m_predicatesPosition != -1)
	{
		parser.seek(m_predicatesPosition);
		parsePredicateSection();
	}

	for (size_t i = 0; i < m_actionPositions.size(); i++)
		if (m_actionPositions[i] != -1)
		{
			parser.seek(m_actionPositions[i]);
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

void Domain::parseRequirementSection()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");
	parser.expect<std::string>("requirements");

	while (parser.currentCharacter() != ')')
	{
		parser.expect<std::string>(":");

		m_requirements.emplace_back(Requirement::parse(m_context));

		parser.skipWhiteSpace();
	}

	// TODO: do this check only once the problem is parsed
	// If no requirements are specified, assume STRIPS
	if (m_requirements.empty())
		m_requirements.emplace_back(Requirement::Type::STRIPS);

	parser.expect<std::string>(")");
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

	m_context.logger.logWarning(m_context.parser.coordinate(), "requirement “" + Requirement(requirementType).toPDDL() + "” used but never declared");

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
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");
	parser.expect<std::string>("types");

	checkRequirement(Requirement::Type::Typing);

	parser.skipWhiteSpace();

	// Store types and their parent types
	while (parser.currentCharacter() != ')')
	{
		if (parser.currentCharacter() == '(')
			throw utils::ParserException(parser.coordinate(), "only primitive types are allowed in type section");

		expressions::PrimitiveType::parseTypedDeclaration(m_context, *this);

		parser.skipWhiteSpace();
	}

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseConstantSection()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");
	parser.expect<std::string>("constants");

	// Store constants
	expressions::Constant::parseTypedDeclarations(m_context, *this);

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parsePredicateSection()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");
	parser.expect<std::string>("predicates");

	parser.skipWhiteSpace();

	// Store predicates and their arguments
	while (parser.currentCharacter() != ')')
	{
		expressions::PredicateDeclaration::parse(m_context, *this);

		parser.skipWhiteSpace();
	}

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseActionSection()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");
	parser.expect<std::string>("action");

	Action::parseDeclaration(m_context, *this);

	parser.expect<std::string>(")");
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
			predicate->normalize();
		});

	std::for_each(m_actions.begin(), m_actions.end(),
		[](auto &action)
		{
			action->normalize();
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
