#include <plasp/pddl/Action.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>

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

Action &Action::parseDeclaration(utils::Parser &parser, Context &context)
{
	const auto actionName = parser.parseIdentifier(isIdentifier);

	auto action = std::make_unique<Action>(Action(actionName));

	parser.expect<std::string>(":parameters");

	parser.skipWhiteSpace();

	// Read parameters
	while (parser.currentCharacter() != ':')
	{
		Variable::parseTyped(parser, context, action->m_parameters);

		parser.skipWhiteSpace();
	}

	// Parse preconditions and effects
	while (parser.currentCharacter() == ')')
	{
		parser.expect<std::string>(":");

		const auto sectionIdentifier = parser.parseIdentifier(isIdentifier);

		if (sectionIdentifier == "precondition")
			action->parsePrecondition(parser, context);
	}

	parser.expect<std::string>(")");

	// Store new action
	context.actions.emplace_back(std::move(action));

	return *context.actions.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Action::parsePrecondition(utils::Parser &parser, Context &context)
{
	parser.skipWhiteSpace();

	// Check for empty precondition
	if (parser.currentCharacter() == '(')
	{
		// Leave the preconditions empty and return
		parser.expect<std::string>("()");
		parser.expect<std::string>(")");

		return;
	}

	parser.expect<std::string>")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Action::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variables &Action::parameters() const
{
	return m_parameters;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
