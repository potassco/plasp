#include <plasp/pddl/expressions/Variable.h>

#include <algorithm>

#include <boost/assert.hpp>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/ExpressionVisitor.h>
#include <plasp/pddl/Identifier.h>
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
:	m_isDirty{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::parseDeclaration(Context &context, Variables &parameters)
{
	context.parser.skipWhiteSpace();

	context.parser.expect<std::string>("?");

	auto variable = std::make_unique<Variable>(Variable());

	variable->m_name = context.parser.parseIdentifier(isIdentifier);

	// Flag variable for potentially upcoming type declaration
	variable->setDirty();

	parameters.emplace_back(std::move(variable));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::parseTypedDeclaration(Context &context, ExpressionContext &expressionContext)
{
	auto &variables = expressionContext.parameters;

	// Parse and store variable itself
	parseDeclaration(context, variables);

	auto &variable = variables.back();

	context.parser.skipWhiteSpace();

	// Check if the variable has a type declaration
	if (!context.parser.advanceIf('-'))
		return;

	// TODO: do not allow nested either expressions

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

	context.parser.skipWhiteSpace();

	// Parse argument of "either" type (always begins with opening parenthesis)
	if (context.parser.currentCharacter() == '(')
	{
		context.parser.expect<std::string>("(");
		context.parser.expect<std::string>("either");

		variable->m_eitherExpression = Either::parse(context, expressionContext, parseExistingPrimitiveType);

		context.parser.expect<std::string>(")");

		setType(variable->m_eitherExpression.get());
		return;
	}

	// Parse primitive type
	const auto *type = PrimitiveType::parseAndFindOrCreate(context, expressionContext.domain);

	setType(type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable *Variable::parseAndFind(Context &context, const ExpressionContext &expressionContext)
{
	context.parser.skipWhiteSpace();

	context.parser.expect<std::string>("?");

	const auto variableName = context.parser.parseIdentifier(isIdentifier);

	const auto &variables = expressionContext.parameters;

	const auto match = std::find_if(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			return variable->name() == variableName;
		});

	if (match == variables.cend())
		throw utils::ParserException(context.parser, "Parameter \"" + variableName + "\" used but never declared");

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
