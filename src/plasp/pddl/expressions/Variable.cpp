#include <plasp/pddl/expressions/Variable.h>

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
// Variable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Variable::Variable()
:	m_isDirty{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VariablePointer Variable::parseDeclaration(utils::Parser &parser)
{
	parser.skipWhiteSpace();

	parser.expect<std::string>("?");

	auto variable = std::make_unique<Variable>(Variable());

	variable->m_name = parser.parseIdentifier(isIdentifier);
	variable->setDirty();

	return variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::parseTypedDeclaration(utils::Parser &parser, Context &context, Variables &variables)
{
	// Parse and store variable itself
	variables.emplace_back(parseDeclaration(parser));

	parser.skipWhiteSpace();

	// Check if the variable has a type declaration
	if (!parser.advanceIf('-'))
		return;

	// Parse argument type
	const auto type = parseType(parser, context);

	// Set the argument type for all previously flagged arguments
	std::for_each(variables.begin(), variables.end(),
		[&](auto &variable)
		{
			if (!variable->isDirty())
				return;

			variable->setType(type);
			variable->setDirty(false);
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable *Variable::parse(utils::Parser &parser, const Variables &variables)
{
	parser.skipWhiteSpace();

	parser.expect<std::string>("?");

	const auto variableName = parser.parseIdentifier(isIdentifier);

	const auto match = std::find_if(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			return variable->name() == variableName;
		});

	if (match == variables.cend())
		throw utils::ParserException(parser.row(), parser.column(), "Variable \"" + variableName + "\" used but never declared");

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

TypePtr Variable::type() const
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

void Variable::setType(TypePtr type)
{
	m_type = type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
