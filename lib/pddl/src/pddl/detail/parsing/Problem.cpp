#include <pddl/detail/parsing/Problem.h>

#include <pddl/Exception.h>
#include <pddl/detail/Requirements.h>
#include <pddl/detail/parsing/ConstantDeclaration.h>
#include <pddl/detail/parsing/InitialState.h>
#include <pddl/detail/parsing/Precondition.h>
#include <pddl/detail/parsing/Requirement.h>
#include <pddl/detail/parsing/Unsupported.h>
#include <pddl/detail/parsing/Utils.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Problem
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ProblemParser::ProblemParser(Context &context, ast::Domain &domain)
:	m_context{context},
	m_domain{domain},
	m_domainPosition{tokenize::InvalidStreamPosition},
	m_requirementsPosition{tokenize::InvalidStreamPosition},
	m_objectsPosition{tokenize::InvalidStreamPosition},
	m_initialStatePosition{tokenize::InvalidStreamPosition},
	m_goalPosition{tokenize::InvalidStreamPosition}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::ProblemPointer ProblemParser::parse()
{
	auto problem = std::make_unique<ast::Problem>(&m_domain);

	findSections(*problem);

	auto &tokenizer = m_context.tokenizer;

	if (m_domainPosition == tokenize::InvalidStreamPosition)
		throw ParserException(tokenizer.location(), "problem description does not specify a corresponding domain");

	tokenizer.seek(m_domainPosition);
	parseDomainSection(*problem);

	if (m_requirementsPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_requirementsPosition);
		parseRequirementSection(*problem);
	}

	if (m_objectsPosition != tokenize::InvalidStreamPosition)
	{
		tokenizer.seek(m_objectsPosition);
		parseObjectSection(*problem);
	}

	if (m_initialStatePosition == tokenize::InvalidStreamPosition)
		throw ParserException(tokenizer.location(), "problem description does not specify an initial state");

	tokenizer.seek(m_initialStatePosition);
	parseInitialStateSection(*problem);

	if (m_goalPosition == tokenize::InvalidStreamPosition)
		throw ParserException(tokenizer.location(), "problem description does not specify a goal");

	tokenizer.seek(m_goalPosition);
	parseGoalSection(*problem);

	return problem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ProblemParser::findSections(ast::Problem &problem)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>("define");
	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>("problem");

	problem.name = tokenizer.getIdentifier();

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

	while (tokenizer.currentCharacter() != ')')
	{
		const auto position = tokenizer.position();

		tokenizer.expect<std::string>("(");
		tokenizer.expect<std::string>(":");

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
			throw exceptUnsupportedSection(position, m_context);
		}
		else
		{
			const auto sectionIdentifier = tokenizer.getIdentifier();

			tokenizer.seek(position);
			throw ParserException(tokenizer.location(), "unknown problem section “" + sectionIdentifier + "”");
		}

		// Skip section for now and parse it later
		skipSection(tokenizer);

		tokenizer.skipWhiteSpace();
	}

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ProblemParser::parseDomainSection(ast::Problem &problem)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("domain");

	tokenizer.skipWhiteSpace();

	const auto domainName = tokenizer.getIdentifier();

	if (problem.domain->name != domainName)
		throw ParserException(tokenizer.location(), "domains do not match (“" + problem.domain->name + "” and “" + domainName + "”)");

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ProblemParser::parseRequirementSection(ast::Problem &problem)
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
			problem.requirements.emplace_back(requirement.value());

		tokenizer.skipWhiteSpace();
	}

	// TODO: do this check only once the problem is parsed
	// If no requirements are specified, assume STRIPS
	if (problem.requirements.empty())
		problem.requirements.emplace_back(ast::Requirement::STRIPS);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: refactor, exists identically in DomainParser
void ProblemParser::computeDerivedRequirements(ast::Problem &problem)
{
	const auto addRequirementUnique =
		[&](const auto requirement)
		{
			if (hasRequirement(problem, requirement))
				return;

			problem.requirements.push_back(ast::Requirement(requirement));
		};

	if (hasRequirement(problem, ast::Requirement::ADL))
	{
		addRequirementUnique(ast::Requirement::STRIPS);
		addRequirementUnique(ast::Requirement::Typing);
		addRequirementUnique(ast::Requirement::NegativePreconditions);
		addRequirementUnique(ast::Requirement::DisjunctivePreconditions);
		addRequirementUnique(ast::Requirement::Equality);
		addRequirementUnique(ast::Requirement::QuantifiedPreconditions);
		addRequirementUnique(ast::Requirement::ConditionalEffects);
	}

	if (hasRequirement(problem, ast::Requirement::QuantifiedPreconditions))
	{
		addRequirementUnique(ast::Requirement::ExistentialPreconditions);
		addRequirementUnique(ast::Requirement::UniversalPreconditions);
	}

	if (hasRequirement(problem, ast::Requirement::Fluents))
	{
		addRequirementUnique(ast::Requirement::NumericFluents);
		addRequirementUnique(ast::Requirement::ObjectFluents);
	}

	if (hasRequirement(problem, ast::Requirement::TimedInitialLiterals))
		addRequirementUnique(ast::Requirement::DurativeActions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ProblemParser::parseObjectSection(ast::Problem &problem)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("objects");

	// Store constants
	parseAndAddConstantDeclarations(m_context, problem);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ProblemParser::parseInitialStateSection(ast::Problem &problem)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("init");

	ASTContext astContext(problem);
	VariableStack variableStack;

	problem.initialState = parseInitialState(m_context, astContext, variableStack);
	tokenizer.expect<std::string>(")");

	skipSection(tokenizer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ProblemParser::parseGoalSection(ast::Problem &problem)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("goal");

	ASTContext astContext(problem);
	VariableStack variableStack;

	problem.goal = parsePrecondition(m_context, astContext, variableStack);
	tokenizer.expect<std::string>(")");

	skipSection(tokenizer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
