#include <plasp/pddl/expressions/Variable.h>

#include <algorithm>

#include <boost/assert.hpp>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/expressions/Either.h>
#include <plasp/pddl/expressions/PrimitiveType.h>
#include <plasp/pddl/expressions/Type.h>

#include <tokenize/TokenizerException.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Variable::Variable()
:	m_isDirty{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Variable::Variable(std::string name)
:	m_isDirty{false},
	m_name{name}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::parseDeclaration(Context &context, Variables &parameters)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.skipWhiteSpace();

	tokenizer.expect<std::string>("?");

	auto variable = VariablePointer(new Variable);

	variable->m_name = tokenizer.getIdentifier();

	// Check if variable of that name already exists in the current scope
	const auto match = std::find_if(parameters.cbegin(), parameters.cend(),
		[&](const auto &parameter)
		{
			return parameter->name() == variable->m_name;
		});

	if (match != parameters.cend())
		throw tokenize::TokenizerException(tokenizer.location(), "variable “" + variable->m_name + "” already declared in this scope");

	// Flag variable for potentially upcoming type declaration
	variable->setDirty();

	parameters.emplace_back(variable);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::parseTypedDeclaration(Context &context, ExpressionContext &expressionContext,
		Variables &variables)
{
	auto &tokenizer = context.tokenizer;

	// Parse and store variable itself
	parseDeclaration(context, variables);

	auto variable = variables.back();

	tokenizer.skipWhiteSpace();

	// Check if the variable has a type declaration
	if (!tokenizer.testAndSkip<char>('-'))
		return;

	const auto setType =
		[&](ExpressionPointer type)
		{
			// Set the argument type for all previously flagged arguments
			for (auto &variable : variables)
			{
				if (!variable->isDirty())
					return;

				variable->setType(type);
				variable->setDirty(false);
			}
		};

	tokenizer.skipWhiteSpace();

	// Parse argument if it has "either" type (always begins with opening parenthesis)
	variable->m_type = Either::parse(context, expressionContext, parseExistingPrimitiveType);

	// Else, try parsing it as a primitive type
	if (!variable->m_type)
		variable->m_type = PrimitiveType::parseAndFind(context, expressionContext.domain);

	setType(variable->m_type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::parseTypedDeclarations(Context &context, ExpressionContext &expressionContext,
	Variables &variables)
{
	auto &tokenizer = context.tokenizer;

	while (tokenizer.currentCharacter() != ')')
	{
		parseTypedDeclaration(context, expressionContext, variables);

		tokenizer.skipWhiteSpace();
	}

	if (variables.empty())
		return;

	// Check correct use of typing requirement
	const auto typingUsed = (variables.back()->type() != nullptr);

	// If types are given, check that typing is a requirement
	if (typingUsed)
		expressionContext.checkRequirement(Requirement::Type::Typing);
	// If no types are given, check that typing is not a requirement
	else if (expressionContext.hasRequirement(Requirement::Type::Typing))
		throw tokenize::TokenizerException(tokenizer.location(), "variable has undeclared type");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::setName(std::string name)
{
	m_name = name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Variable::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::setType(ExpressionPointer type)
{
	m_type = type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Variable::type() const
{
	return m_type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::setDirty(bool isDirty)
{
	m_isDirty = isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Variable::isDirty() const
{
	return m_isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::collectParameters(std::set<VariablePointer> &parameters)
{
	parameters.emplace(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::print(std::ostream &ostream) const
{
	ostream << "?" << m_name;

	if (m_type)
	{
		ostream << " - ";
		m_type->print(ostream);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
