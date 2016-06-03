#include <plasp/pddl/expressions/Predicate.h>

#include <plasp/pddl/ExpressionVisitor.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

PredicatePointer Predicate::parse(std::string name, utils::Parser &parser,
	Context &context, const Variables &parameters)
{
	auto expression = std::make_unique<Predicate>(Predicate());

	expression->m_name = name;

	parser.skipWhiteSpace();

	// Parse arguments
	while (parser.currentCharacter() != ')')
		expression->m_arguments.emplace_back(Variable::parse(parser, parameters));

	// TODO: check that signature matches one of the declared ones

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Predicate::accept(plasp::pddl::ExpressionVisitor &expressionVisitor) const
{
	expressionVisitor.visit(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<const Variable *> &Predicate::arguments() const
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
