#include <plasp/pddl/Problem.h>

#include <algorithm>

#include <plasp/pddl/Identifier.h>
#include <plasp/utils/IO.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Problem
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Problem::Problem(Context &context)
:	m_context(context)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Problem Problem::fromPDDL(Context &context)
{
	Problem problem(context);

	problem.m_name = context.parser.parseIdentifier(isIdentifier);

	std::cout << "Parsing problem " << problem.m_name << std::endl;

	context.parser.expect<std::string>(")");

	while (true)
	{
		context.parser.skipWhiteSpace();

		if (context.parser.currentCharacter() == ')')
			break;

		problem.parseSection();
	}

	problem.computeDerivedRequirements();

	problem.checkConsistency();

	return problem;
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

void Problem::parseSection()
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
	if (sectionIdentifier == "domain")
		skipSection();
	else if (sectionIdentifier == "requirements")
		parseRequirementSection();
	else if (sectionIdentifier == "objects")
		skipSection();
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

void Problem::parseRequirementSection()
{
	m_context.parser.skipWhiteSpace();

	while (m_context.parser.currentCharacter() != ')')
	{
		m_context.parser.expect<std::string>(":");

		m_requirements.emplace_back(Requirement::parse(m_context));

		m_context.parser.skipWhiteSpace();
	}

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

void Problem::checkConsistency()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
