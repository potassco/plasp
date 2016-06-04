#include <plasp/pddl/Domain.h>

#include <algorithm>

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Identifier.h>
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
:	m_context(context)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Domain Domain::fromPDDL(utils::Parser &parser, Context &context)
{
	Domain domain(context);

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

void Domain::parseSection(utils::Parser &parser)
{
	parser.expect<std::string>("(");
	parser.expect<std::string>(":");

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

	// TODO: check order of the sections
	if (sectionIdentifier == "requirements")
		parseRequirementSection(parser);
	else if (sectionIdentifier == "types")
		parseTypeSection(parser);
	else if (sectionIdentifier == "constants")
		parseConstantSection(parser);
	else if (sectionIdentifier == "predicates")
		parsePredicateSection(parser);
	else if (sectionIdentifier == "functions")
		skipSection();
	else if (sectionIdentifier == "constraints")
		skipSection();
	else if (sectionIdentifier == "action")
		parseActionSection(parser);
	else if (sectionIdentifier == "durative-action")
		skipSection();
	else if (sectionIdentifier == "derived")
		skipSection();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseRequirementSection(utils::Parser &parser)
{
	parser.skipWhiteSpace();

	while (parser.currentCharacter() != ')')
	{
		if (parser.currentCharacter() == ':')
			parser.advance();

		m_requirements.emplace_back(Requirement::parse(parser));

		parser.skipWhiteSpace();
	}

	if (m_requirements.empty())
		throw utils::ParserException(parser.row(), parser.column(), "Requirements section does not contain any requirements");

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

void Domain::computeDerivedRequirements()
{
	const auto addRequirementUnique =
		[&](const auto requirement)
		{
			if (hasRequirement(requirement))
				return;

			m_requirements.push_back(Requirement(requirement));
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

void Domain::parseTypeSection(utils::Parser &parser)
{
	parser.skipWhiteSpace();

	// Store types and their parent types
	while (parser.currentCharacter() != ')')
	{
		if (parser.currentCharacter() == '(')
			throw utils::ParserException(parser.row(), parser.column(), "Only primitive types are allowed in type section");

		expressions::PrimitiveType::parseTypedDeclaration(parser, m_context);

		parser.skipWhiteSpace();
	}

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseConstantSection(utils::Parser &parser)
{
	parser.skipWhiteSpace();

	// Store constants
	while (parser.currentCharacter() != ')')
	{
		expressions::Constant::parseTypedDeclaration(parser, m_context);

		parser.skipWhiteSpace();
	}

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parsePredicateSection(utils::Parser &parser)
{
	parser.skipWhiteSpace();

	// Store predicates and their arguments
	while (parser.currentCharacter() != ')')
	{
		expressions::PredicateDeclaration::parse(parser, m_context);

		parser.skipWhiteSpace();
	}

	parser.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Domain::parseActionSection(utils::Parser &parser)
{
	parser.skipWhiteSpace();

	Action::parseDeclaration(parser, m_context);

	parser.expect<std::string>(")");
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
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
