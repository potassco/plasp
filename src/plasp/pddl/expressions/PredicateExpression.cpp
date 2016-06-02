#include <plasp/pddl/expressions/PredicateExpression.h>

#include <plasp/pddl/ExpressionVisitor.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PredicateExpression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<PredicateExpression> PredicateExpression::parse(std::string name, utils::Parser &parser, Context &context, const Variables &parameters)
{
	auto expression = std::make_unique<PredicateExpression>(PredicateExpression());

	expression->m_name = name;

	parser.skipWhiteSpace();

	// Parse arguments
	while (parser.currentCharacter() != ')')
		Variable::parseTyped(parser, context, expression->m_arguments);

	// TODO: check that signature matches one of the declared ones

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PredicateExpression::accept(plasp::pddl::ExpressionVisitor &expressionVisitor) const
{
	expressionVisitor.visit(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
