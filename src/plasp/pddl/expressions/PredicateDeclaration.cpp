#include <plasp/pddl/expressions/PredicateDeclaration.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/ExpressionVisitor.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/Constant.h>
#include <plasp/pddl/expressions/Reference.h>
#include <plasp/pddl/expressions/Variable.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PredicateDeclaration
//
////////////////////////////////////////////////////////////////////////////////////////////////////

PredicateDeclaration::PredicateDeclaration()
:	m_isDeclared{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PredicateDeclaration::parse(utils::Parser &parser, Context &context)
{
	parser.expect<std::string>("(");

	auto predicate = std::make_unique<PredicateDeclaration>(PredicateDeclaration());

	predicate->m_name = parser.parseIdentifier(isIdentifier);

	// Flag predicate as correctly declared in the types section
	predicate->setDeclared();

	parser.skipWhiteSpace();

	// Parse arguments
	while (parser.currentCharacter() != ')')
	{
		expressions::Variable::parseTypedDeclaration(parser, context, predicate->m_arguments);

		parser.skipWhiteSpace();
	}

	parser.expect<std::string>(")");

	// Store new predicate
	context.predicateDeclarations.emplace_back(std::move(predicate));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PredicateDeclaration::accept(plasp::pddl::ExpressionVisitor &expressionVisitor) const
{
	expressionVisitor.visit(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PredicateDeclaration::setDeclared()
{
	m_isDeclared = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool PredicateDeclaration::isDeclared() const
{
	return m_isDeclared;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &PredicateDeclaration::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variables &PredicateDeclaration::arguments() const
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
