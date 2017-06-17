#include <pddlparse/detail/parsing/Action.h>

#include <pddlparse/AST.h>
#include <pddlparse/ParserException.h>
#include <pddlparse/detail/ASTContext.h>
#include <pddlparse/detail/VariableStack.h>
#include <pddlparse/detail/parsing/Effect.h>
#include <pddlparse/detail/parsing/Precondition.h>
#include <pddlparse/detail/parsing/Utils.h>
#include <pddlparse/detail/parsing/VariableDeclaration.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Action
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ActionParser::ActionParser(Context &context, ast::Domain &domain)
:	m_context{context},
	m_domain{domain},
	m_parametersPosition{-1},
	m_preconditionPosition{-1},
	m_effectPosition{-1},
	m_varsPosition{-1}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::ActionPointer ActionParser::parse()
{
	auto action = std::make_unique<ast::Action>();

	findSections(*action);

	auto &tokenizer = m_context.tokenizer;

	if (m_parametersPosition != -1)
	{
		tokenizer.seek(m_parametersPosition);
		parseParameterSection(*action);
	}

	// For compatibility with old PDDL versions, vars sections are parsed in addition to parameters
	if (m_varsPosition != -1)
	{
		tokenizer.seek(m_varsPosition);
		parseVarsSection(*action);
	}

	if (m_preconditionPosition != -1)
	{
		tokenizer.seek(m_preconditionPosition);
		parsePreconditionSection(*action);
	}

	if (m_effectPosition != -1)
	{
		tokenizer.seek(m_effectPosition);
		parseEffectSection(*action);
	}

	return action;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ActionParser::findSections(ast::Action &action)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("action");

	action.name = tokenizer.getIdentifier();

	// TODO: rename parameters appropriately
	const auto setSectionPosition =
		[&](const std::string &sectionName, auto &sectionPosition, const auto value, bool unique = false)
		{
			if (unique && sectionPosition != -1)
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

		tokenizer.expect<std::string>(":");

		if (tokenizer.testIdentifierAndSkip("parameters"))
			setSectionPosition("parameters", m_parametersPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("precondition"))
			setSectionPosition("precondition", m_preconditionPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("effect"))
			setSectionPosition("effect", m_effectPosition, position, true);
		else if (tokenizer.testIdentifierAndSkip("vars"))
			setSectionPosition("vars", m_varsPosition, position, true);
		else
		{
			const auto sectionIdentifier = tokenizer.getIdentifier();

			tokenizer.seek(position);
			throw ParserException(tokenizer.location(), "unknown action section “" + sectionIdentifier + "”");
		}

		tokenizer.expect<std::string>("(");

		// Skip section for now and parse it later
		skipSection(tokenizer);

		tokenizer.skipWhiteSpace();
	}

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ActionParser::parseParameterSection(ast::Action &action)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>(":parameters");
	tokenizer.expect<std::string>("(");

	parseAndAddVariableDeclarations(m_context, m_domain, action.parameters);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ActionParser::parsePreconditionSection(ast::Action &action)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>(":precondition");

	ASTContext astContext(m_domain);
	VariableStack variableStack;
	variableStack.push(&action.parameters);

	action.precondition = parsePrecondition(m_context, astContext, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ActionParser::parseEffectSection(ast::Action &action)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>(":effect");

	ASTContext astContext(m_domain);
	VariableStack variableStack;
	variableStack.push(&action.parameters);

	action.effect = parseEffect(m_context, astContext, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ActionParser::parseVarsSection(ast::Action &action)
{
	auto &tokenizer = m_context.tokenizer;

	tokenizer.expect<std::string>(":vars");
	tokenizer.expect<std::string>("(");

	m_context.warningCallback(tokenizer.location(), "“vars” section is not part of the PDDL 3.1 specification, treating it like additional “parameters” section");

	parseAndAddVariableDeclarations(m_context, m_domain, action.parameters);

	tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
