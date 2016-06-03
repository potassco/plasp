#include <plasp/pddl/expressions/VariableExpression.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/ExpressionVisitor.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VariableExpression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

VariableExpression::VariableExpression()
:	m_isDirty{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VariableExpressionPointer VariableExpression::parseDeclaration(utils::Parser &parser)
{
	parser.skipWhiteSpace();

	parser.expect<std::string>("?");

	auto variable = std::make_unique<VariableExpression>(VariableExpression());

	variable->m_name = parser.parseIdentifier(isIdentifier);
	variable->setDirty();

	return variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void VariableExpression::parseTypedDeclaration(utils::Parser &parser, Context &context,
	VariableExpressions &variableExpressions)
{
	// Parse and store variable itself
	variableExpressions.emplace_back(parseDeclaration(parser));

	parser.skipWhiteSpace();

	// Check if the variable has a type declaration
	if (!parser.advanceIf('-'))
		return;

	// Parse argument type
	const auto type = parseType(parser, context);

	// Set the argument type for all previously flagged arguments
	std::for_each(variableExpressions.begin(), variableExpressions.end(),
		[&](auto &variable)
		{
			if (!variable->isDirty())
				return;

			variable->setType(type);
			variable->setDirty(false);
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const VariableExpression *VariableExpression::parse(utils::Parser &parser,
	const VariableExpressions &variableExpressions)
{
	parser.skipWhiteSpace();

	parser.expect<std::string>("?");

	const auto variableName = parser.parseIdentifier(isIdentifier);

	const auto match = std::find_if(variableExpressions.cbegin(), variableExpressions.cend(),
		[&](const auto &variableExpression)
		{
			return variableExpression->name() == variableName;
		});

	if (match == variableExpressions.cend())
		throw utils::ParserException(parser.row(), parser.column(), "Variable \"" + variableName + "\" used but never declared");

	return match->get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void VariableExpression::accept(plasp::pddl::ExpressionVisitor &expressionVisitor) const
{
	expressionVisitor.visit(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &VariableExpression::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TypePtr VariableExpression::type() const
{
	return m_type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void VariableExpression::setDirty(bool isDirty)
{
	m_isDirty = isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool VariableExpression::isDirty() const
{
	return m_isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void VariableExpression::setType(TypePtr type)
{
	m_type = type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
