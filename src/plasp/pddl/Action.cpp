#include <plasp/pddl/Action.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/Type.h>
#include <plasp/utils/IO.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Action
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void Action::parseDeclaration(Context &context, Domain &domain)
{
	auto action = std::make_unique<Action>(Action());

	action->m_name = context.parser.parseIdentifier(isIdentifier);

	context.parser.expect<std::string>(":parameters");
	context.parser.expect<std::string>("(");

	ExpressionContext expressionContext(domain, action->m_parameters);

	// Read parameters
	expressions::Variable::parseTypedDeclarations(context, expressionContext);

	context.parser.expect<std::string>(")");

	// Parse preconditions and effects
	while (context.parser.currentCharacter() != ')')
	{
		context.parser.expect<std::string>(":");

		if (context.parser.probe<std::string>("precondition"))
			action->m_precondition = parsePreconditionExpression(context, expressionContext);
		else if (context.parser.probe<std::string>("effect"))
			action->m_effect = parseEffectExpression(context, expressionContext);

		context.parser.skipWhiteSpace();
	}

	// Store new action
	expressionContext.domain.actions().emplace_back(std::move(action));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Action::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const expressions::Variables &Action::parameters() const
{
	return m_parameters;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Expression &Action::precondition() const
{
	BOOST_ASSERT(m_precondition);

	return *m_precondition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Expression &Action::effect() const
{
	BOOST_ASSERT(m_effect);

	return *m_effect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
