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

PredicateExpressionPointer PredicateExpression::parse(std::string name, utils::Parser &parser,
	Context &context, const VariableExpressions &parameters)
{
	auto expression = std::make_unique<PredicateExpression>(PredicateExpression());

	expression->m_name = name;

	parser.skipWhiteSpace();

	// Parse arguments
	while (parser.currentCharacter() != ')')
		expression->m_arguments.emplace_back(VariableExpression::parse(parser, parameters));

	// TODO: check that signature matches one of the declared ones

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PredicateExpression::accept(plasp::pddl::ExpressionVisitor &expressionVisitor) const
{
	expressionVisitor.visit(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<const VariableExpression *> &PredicateExpression::arguments() const
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
