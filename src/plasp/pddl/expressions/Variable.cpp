#include <plasp/pddl/expressions/Variable.h>

#include <algorithm>

#include <boost/assert.hpp>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/expressions/Either.h>
#include <plasp/pddl/expressions/PrimitiveType.h>
#include <plasp/pddl/expressions/Type.h>
#include <plasp/utils/ParserException.h>

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
:	m_isDirty{false},
	m_type{nullptr}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::parseDeclaration(Context &context, Variables &parameters)
{
	auto &parser = context.parser;

	parser.skipWhiteSpace();

	parser.expect<std::string>("?");

	auto variable = std::make_unique<Variable>(Variable());

	variable->m_name = parser.parseIdentifier();

	// Check if variable of that name already exists in the current scope
	const auto match = std::find_if(parameters.cbegin(), parameters.cend(),
		[&](const auto &parameter)
		{
			return parameter->name() == variable->m_name;
		});

	if (match != parameters.cend())
		throw utils::ParserException(parser.coordinate(), "variable “" + variable->m_name + "” already declared in this scope");

	// Flag variable for potentially upcoming type declaration
	variable->setDirty();

	parameters.emplace_back(std::move(variable));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::parseTypedDeclaration(Context &context, ExpressionContext &expressionContext)
{
	auto &parser = context.parser;
	auto &variables = expressionContext.parameters;

	// Parse and store variable itself
	parseDeclaration(context, variables);

	auto &variable = variables.back();

	parser.skipWhiteSpace();

	// Check if the variable has a type declaration
	if (!parser.testAndSkip<char>('-'))
		return;

	const auto setType =
		[&](const auto *type)
		{
			// Set the argument type for all previously flagged arguments
			std::for_each(variables.begin(), variables.end(),
				[&](auto &variable)
				{
					if (!variable->isDirty())
						return;

					variable->setType(type);
					variable->setDirty(false);
				});
		};

	parser.skipWhiteSpace();

	// Parse argument of "either" type (always begins with opening parenthesis)
	if ((variable->m_eitherExpression = Either::parse(context, expressionContext, parseExistingPrimitiveType)))
	{
		setType(variable->m_eitherExpression.get());
		return;
	}

	// Parse primitive type
	const auto *type = PrimitiveType::parseAndFind(context, expressionContext.domain);

	setType(type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::parseTypedDeclarations(Context &context, ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	while (parser.currentCharacter() != ')')
	{
		parseTypedDeclaration(context, expressionContext);

		parser.skipWhiteSpace();
	}

	if (expressionContext.parameters.empty())
		return;

	// Check correct use of typing requirement
	const auto typingUsed = (expressionContext.parameters.back()->type() != nullptr);

	// If types are given, check that typing is a requirement
	if (typingUsed)
		expressionContext.checkRequirement(Requirement::Type::Typing);
	// If no types are given, check that typing is not a requirement
	else if (expressionContext.hasRequirement(Requirement::Type::Typing))
		throw utils::ParserException(parser.coordinate(), "variable has undeclared type");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable *Variable::parseAndFind(Context &context, const ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	parser.skipWhiteSpace();

	parser.expect<std::string>("?");

	const auto variableName = parser.parseIdentifier();

	const auto &variables = expressionContext.parameters;

	const auto match = std::find_if(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			return variable->name() == variableName;
		});

	if (match == variables.cend())
		throw utils::ParserException(parser.coordinate(), "parameter “" + variableName + "” used but never declared");

	return match->get();
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

const Expression *Variable::type() const
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

void Variable::setType(const Expression *type)
{
	m_type = type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Variable::normalize()
{
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
