#include <plasp/pddl/expressions/PredicateDeclaration.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/expressions/Constant.h>
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

void PredicateDeclaration::parse(Context &context, Domain &domain)
{
	context.parser.expect<std::string>("(");

	auto predicate = std::make_unique<PredicateDeclaration>(PredicateDeclaration());

	predicate->m_name = context.parser.parseIdentifier();

	// Flag predicate as correctly declared in the types section
	predicate->setDeclared();

	context.parser.skipWhiteSpace();

	ExpressionContext expressionContext(domain, predicate->m_parameters);

	// Parse arguments
	expressions::Variable::parseTypedDeclarations(context, expressionContext);

	context.parser.expect<std::string>(")");

	domain.predicates().emplace_back(std::move(predicate));
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
	return m_parameters;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer PredicateDeclaration::normalize()
{
	for (size_t i = 0; i < m_parameters.size(); i++)
		m_parameters[i]->setName("X" + std::to_string(i));

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
