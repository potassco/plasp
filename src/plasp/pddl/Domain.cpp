#include <plasp/pddl/Domain.h>

#include <algorithm>

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Identifier.h>
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

Domain Domain::fromPDDL(Context &context)
{
	Domain domain(context);

	domain.m_name = context.parser.parseIdentifier(isIdentifier);

	std::cout << "Parsing domain " << domain.m_name << std::endl;

	context.parser.expect<std::string>(")");

	while (true)
	{
		context.parser.skipWhiteSpace();

		if (context.parser.currentCharacter() == ')')
			break;

		domain.parseSection();
	}

	domain.computeDerivedRequirements();

	domain.checkConsistency();

	return domain;
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

const expressions::PrimitiveTypes &Domain::types() const
{
	return m_context.primitiveTypes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const expressions::Constants &Domain::constants() const
{
	return m_context.constants;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const expressions::PredicateDeclarations &Domain::predicates() const
{
	return m_context.predicateDeclarations;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<std::unique_ptr<Action>> &Domain::actions() const
{
	return m_context.actions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseSection()
{
	m_context.parser.expect<std::string>("(");
	m_context.parser.expect<std::string>(":");

	const auto sectionIdentifier = utils::toLowerCase(m_context.parser.parseIdentifier(isIdentifier));

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
	if (sectionIdentifier == "requirements")
		parseRequirementSection();
	else if (sectionIdentifier == "types")
		parseTypeSection();
	else if (sectionIdentifier == "constants")
		parseConstantSection();
	else if (sectionIdentifier == "predicates")
		parsePredicateSection();
	else if (sectionIdentifier == "functions")
		skipSection();
	else if (sectionIdentifier == "constraints")
		skipSection();
	else if (sectionIdentifier == "action")
		parseActionSection();
	else if (sectionIdentifier == "durative-action")
		skipSection();
	else if (sectionIdentifier == "derived")
		skipSection();
	else
		throw utils::ParserException(m_context.parser, "Unknown domain section \"" + sectionIdentifier + "\"");
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

	// TODO: Do this check only once the problem is parsed
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
	m_context.parser.skipWhiteSpace();

	// Store types and their parent types
	while (m_context.parser.currentCharacter() != ')')
	{
		if (m_context.parser.currentCharacter() == '(')
			throw utils::ParserException(m_context.parser, "Only primitive types are allowed in type section");

		expressions::PrimitiveType::parseTypedDeclaration(m_context);

		m_context.parser.skipWhiteSpace();
	}

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseConstantSection()
{
	m_context.parser.skipWhiteSpace();

	// Store constants
	while (m_context.parser.currentCharacter() != ')')
	{
		expressions::Constant::parseTypedDeclaration(m_context);

		m_context.parser.skipWhiteSpace();
	}

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parsePredicateSection()
{
	m_context.parser.skipWhiteSpace();

	// Store predicates and their arguments
	while (m_context.parser.currentCharacter() != ')')
	{
		expressions::PredicateDeclaration::parse(m_context);

		m_context.parser.skipWhiteSpace();
	}

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseActionSection()
{
	m_context.parser.skipWhiteSpace();

	Action::parseDeclaration(m_context);

	m_context.parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::checkConsistency()
{
	// Verify that typing requirement is correctly declared if used
	if (!m_context.primitiveTypes.empty() && !hasRequirement(Requirement::Type::Typing))
	{
		throw ConsistencyException("Domain contains typing information but does not declare typing requirement");

		m_requirements.push_back(Requirement(Requirement::Type::Typing));
	}

	// Verify that all used types have been declared
	std::for_each(m_context.primitiveTypes.cbegin(), m_context.primitiveTypes.cend(),
		[&](const auto &type)
		{
			if (!type->isDeclared())
				throw ConsistencyException("Type \"" + type->name() + "\" used but never declared");
		});

	// Verify that all used constants have been declared
	std::for_each(m_context.constants.cbegin(), m_context.constants.cend(),
		[&](const auto &constant)
		{
			if (!constant->isDeclared())
				throw ConsistencyException("Constant \"" + constant->name() + "\" used but never declared");

			if (constant->type() == nullptr)
				throw ConsistencyException("Constant \"" + constant->name() + "\" has an undeclared type");
		});

	// Verify that all used predicates have been declared
	std::for_each(m_context.predicateDeclarations.cbegin(), m_context.predicateDeclarations.cend(),
		[&](const auto &predicate)
		{
			if (!predicate->isDeclared())
				throw ConsistencyException("Predicate \"" + predicate->name() + "\" used but never declared");
		});

	// Verify that all variables have types
	// Verify that constants are unique
	// Verify that all primitive types are unique
	// Check for case-sensitivity issues
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
