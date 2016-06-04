#include <plasp/pddl/Action.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/Type.h>
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

Action::Action(std::string name)
:	m_name{name}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Action &Action::parseDeclaration(Context &context)
{
	const auto actionName = context.parser.parseIdentifier(isIdentifier);

	auto action = std::make_unique<Action>(Action(actionName));

	context.parser.expect<std::string>(":parameters");

	context.parser.expect<std::string>("(");

	// Read parameters
	while (context.parser.currentCharacter() != ')')
	{
		expressions::Variable::parseTypedDeclaration(context, action->m_parameters);

		context.parser.skipWhiteSpace();
	}

	context.parser.expect<std::string>(")");

	// Parse preconditions and effects
	while (context.parser.currentCharacter() != ')')
	{
		context.parser.expect<std::string>(":");

		const auto sectionIdentifier = context.parser.parseIdentifier(isIdentifier);

		if (sectionIdentifier == "precondition")
			action->m_precondition = parsePreconditionExpression(context, action->m_parameters);
		else if (sectionIdentifier == "effect")
			action->m_effect = parseEffectExpression(context, action->m_parameters);

		context.parser.skipWhiteSpace();
	}

	// Store new action
	context.actions.emplace_back(std::move(action));

	return *context.actions.back();
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
