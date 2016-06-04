#include <plasp/pddl/expressions/Variable.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/ExpressionVisitor.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/Either.h>
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
:	m_isDirty{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VariablePointer Variable::parseDeclaration(Context &context)
{
	context.parser.skipWhiteSpace();

	context.parser.expect<std::string>("?");

	auto variable = std::make_unique<Variable>(Variable());

	variable->m_name = context.parser.parseIdentifier(isIdentifier);

	// Flag variable for potentially upcoming type declaration
	variable->setDirty();

	return variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::parseTypedDeclaration(Context &context, Variables &parameters)
{
	// Parse and store variable itself
	parameters.emplace_back(parseDeclaration(context));

	auto &parameter = parameters.back();

	context.parser.skipWhiteSpace();

	// Check if the variable has a type declaration
	if (!context.parser.advanceIf('-'))
		return;

	// TODO: do not allow nested either expressions

	const auto setType =
		[&](const auto *type)
		{
			// Set the argument type for all previously flagged arguments
			std::for_each(parameters.begin(), parameters.end(),
				[&](auto &parameter)
				{
					if (!parameter->isDirty())
						return;

					parameter->setType(type);
					parameter->setDirty(false);
				});
		};

	context.parser.skipWhiteSpace();

	// Parse argument of "either" type (always begins with opening parenthesis)
	if (context.parser.currentCharacter() == '(')
	{
		context.parser.expect<std::string>("(");
		context.parser.expect<std::string>("either");

		parameter->m_eitherExpression = Either::parse(context, parameters, parseExistingPrimitiveType);

		context.parser.expect<std::string>(")");

		setType(parameter->m_eitherExpression.get());
		return;
	}

	// Parse primitive type
	const auto *type = PrimitiveType::parseExisting(context);

	setType(type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable *Variable::parseExisting(Context &context, const Variables &variables)
{
	context.parser.skipWhiteSpace();

	context.parser.expect<std::string>("?");

	const auto variableName = context.parser.parseIdentifier(isIdentifier);

	const auto match = std::find_if(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			return variable->name() == variableName;
		});

	if (match == variables.cend())
		throw utils::ParserException(context.parser, "Variable \"" + variableName + "\" used but never declared");

	return match->get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::accept(plasp::pddl::ExpressionVisitor &expressionVisitor) const
{
	expressionVisitor.visit(*this);
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

}
}
}
