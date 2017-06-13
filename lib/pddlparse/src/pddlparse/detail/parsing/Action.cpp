#include <pddlparse/detail/parsing/Action.h>

#include <pddlparse/AST.h>
// TODO: remove
#include <pddlparse/detail/parsing/Utils.h>
#include <pddlparse/detail/parsing/VariableDeclaration.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Action
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddAction(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	tokenizer.expect<std::string>("action");

	auto action = std::make_unique<ast::Action>();
	action->name = tokenizer.getIdentifier();

	tokenizer.expect<std::string>(":parameters");
	tokenizer.expect<std::string>("(");

	// Read parameters
	action->parameters = parseVariableDeclarations(context, domain);

	tokenizer.expect<std::string>(")");

	// TODO: reimplement
	skipSection(tokenizer);

	/*
	// Parse preconditions and effects
	while (!tokenizer.testAndReturn(')'))
	{
		tokenizer.expect<std::string>(":");

		if (tokenizer.testIdentifierAndSkip("precondition"))
			// TODO: reimplement
			//action->precondition = parsePreconditionExpression(context, expressionContext);
			skipSection(tokenizer);
		else if (tokenizer.testIdentifierAndSkip("effect"))
			// TODO: reimplement
			//action->effect = parseEffectExpression(context, expressionContext);
			skipSection(tokenizer);

		tokenizer.skipWhiteSpace();
	}*/

	// Store new action
	domain.actions.emplace_back(std::move(action));

	//tokenizer.expect<std::string>(")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
