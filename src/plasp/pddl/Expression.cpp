#include <plasp/pddl/Expression.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/NAryExpression.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Expression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Expression> parsePreconditionExpression(utils::Parser &parser, Context &context)
{
	parser.skipWhiteSpace();

	parser.expect<std::string>("(");

	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	std::unique_ptr<Expression> expression;

	throw utils::ParserException(parser.row(), parser.column(), "Expression of type \"" + expressionIdentifier + "\" not allowed in preference declaration");

	//if (expressionIdentifier == "and")
	//	expression = NAry

	parser.expect<std::string>(")");

	return std::move(expression);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
