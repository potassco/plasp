#include <plasp/pddl/Domain.h>

#include <algorithm>

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/IO.h>
#include <plasp/pddl/expressions/Constant.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/PrimitiveType.h>
#include <plasp/pddl/expressions/Variable.h>
#include <plasp/utils/IO.h>
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
:	m_context(context)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::readPDDL()
{
	m_context.parser.expect<std::string>("(");
	m_context.parser.expect<std::string>("define");
	m_context.parser.expect<std::string>("(");
	m_context.parser.expect<std::string>("domain");

	m_name = m_context.parser.parseIdentifier(isIdentifier);

	std::cout << "Parsing domain " << m_name << std::endl;

	m_context.parser.expect<std::string>(")");

	while (true)
	{
		m_context.parser.skipWhiteSpace();

		if (m_context.parser.probe(')'))
			break;

		parseSection();
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
	return m_primitiveTypes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const expressions::PrimitiveTypes &Domain::types() const
{
	return m_primitiveTypes;
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
	return m_predicateDeclarations;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const expressions::PredicateDeclarations &Domain::predicates() const
{
	return m_predicateDeclarations;
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

void Domain::parseSection()
{
	auto &parser = m_context.parser;

	parser.expect<std::string>("(");
	parser.expect<std::string>(":");

	// TODO: check order of the sections
	if (parser.probe<std::string>("requirements"))
		parseRequirementSection();
	else if (parser.probe<std::string>("types"))
		parseTypeSection();
	else if (parser.probe<std::string>("constants"))
		parseConstantSection();
	else if (parser.probe<std::string>("predicates"))
		parsePredicateSection();
	else if (parser.probe<std::string>("action"))
		parseActionSection();
	else if (parser.probe<std::string>("functions")
		|| parser.probe<std::string>("constraints")
		|| parser.probe<std::string>("durative-action")
		|| parser.probe<std::string>("derived"))
	{
		const auto sectionIdentifier = parser.parseIdentifier(isIdentifier);
		std::cout << "Skipping section " << sectionIdentifier << std::endl;
		skipSection(m_context.parser);
	}
	else
	{
		const auto sectionIdentifier = parser.parseIdentifier(isIdentifier);
		throw utils::ParserException(m_context.parser, "Unknown domain section \"" + sectionIdentifier + "\"");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseRequirementSection()
{
	m_context.parser.skipWhiteSpace();

	while (m_context.parser.currentCharacter() != ')')
	{
		m_context.parser.expect<std::string>(":");

		m_requirements.emplace_back(Requirement::parse(m_context));

		m_context.parser.skipWhiteSpace();
	}

	// TODO: do this check only once the problem is parsed
	// If no requirements are specified, assume STRIPS
	if (m_requirements.empty())
		m_requirements.emplace_back(Requirement::Type::STRIPS);

	m_context.parser.expect<std::string>(")");
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
	if (!hasRequirement(Requirement::Type::Typing))
		throw utils::ParserException(m_context.parser, "Typing used but not declared as a requirement");

	m_context.parser.skipWhiteSpace();

	// Store types and their parent types
	while (!m_context.parser.probe(')'))
	{
		if (m_context.parser.currentCharacter() == '(')
			throw utils::ParserException(m_context.parser, "Only primitive types are allowed in type section");

		expressions::PrimitiveType::parseTypedDeclaration(m_context, *this);

		m_context.parser.skipWhiteSpace();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseConstantSection()
{
	m_context.parser.skipWhiteSpace();

	// Store constants
	expressions::Constant::parseTypedDeclarations(m_context, *this);

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parsePredicateSection()
{
	m_context.parser.skipWhiteSpace();

	// Store predicates and their arguments
	while (m_context.parser.currentCharacter() != ')')
	{
		expressions::PredicateDeclaration::parse(m_context, *this);

		m_context.parser.skipWhiteSpace();
	}

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseActionSection()
{
	m_context.parser.skipWhiteSpace();

	Action::parseDeclaration(m_context, *this);

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::checkConsistency()
{
	// Verify that constants are unique
	// Verify that all primitive types are unique
	// Check for case-sensitivity issues
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
