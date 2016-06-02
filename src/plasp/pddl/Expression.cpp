#include <plasp/pddl/Expression.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/AndExpression.h>
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

std::unique_ptr<Expression> parsePreconditionExpression(utils::Parser &parser, Context &context, const Variables &parameters)
{
	parser.skipWhiteSpace();

	parser.expect<std::string>("(");

	const auto expressionIdentifier = parser.parseIdentifier(isIdentifier);

	std::cout << "Parsing identifier " << expressionIdentifier << std::endl;

	std::unique_ptr<Expression> expression;

	const auto throwNotAllowed =
		[&]()
		{
			throw utils::ParserException(parser.row(), parser.column(), "Expression of type \"" + expressionIdentifier + "\" not allowed in preference declaration");
		};

	if (expressionIdentifier == "and")
		expression = expressions::AndExpression::parse(parser, context, parameters, parsePreconditionExpression);
	else if (expressionIdentifier == "or")
		throwNotAllowed();
	else if (expressionIdentifier == "not")
		throwNotAllowed();
	else if (expressionIdentifier == "exists")
		throwNotAllowed();
	else
		throw utils::ParserException(parser.row(), parser.column(), "Undeclared expression \"" + expressionIdentifier + "\"");

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
