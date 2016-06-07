#include <plasp/pddl/expressions/Type.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/utils/IO.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InitialState
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void throwUnsupported(const utils::Parser &parser, const std::string &expressionIdentifier)
{
	throw utils::ParserException(parser, "Expression type \"" + expressionIdentifier + "\" currently unsupported in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseInitialState(Context &context)
{
	context.parser.expect<std::string>("(");

	const auto expressionIdentifier = context.parser.parseIdentifier(isIdentifier);

	ExpressionPointer expression;

	if (expressionIdentifier == "at"
		|| expressionIdentifier == "=")
	{
		throwUnsupported(context.parser, expressionIdentifier);
	}

	// TODO: implement predicate parsing

	context.parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
