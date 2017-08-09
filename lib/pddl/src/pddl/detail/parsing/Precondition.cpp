#include <pddl/detail/parsing/Precondition.h>

#include <pddl/AST.h>
#include <pddl/detail/parsing/AtomicFormula.h>
#include <pddl/detail/parsing/Expressions.h>
#include <pddl/detail/parsing/Unsupported.h>
#include <pddl/detail/parsing/Utils.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Precondition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::Precondition> parsePrecondition(Context &context, ASTContext &astContext, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.skipWhiteSpace();

	// Test unsupported expressions first
	const auto position = tokenizer.position();

	tokenizer.expect<std::string>("(");

	if (tokenizer.testIdentifierAndReturn("preference"))
		throw exceptUnsupportedExpression(position, context);

	tokenizer.seek(position);

	// Now, test supported expressions
	std::experimental::optional<ast::Precondition> precondition;

	if ((precondition = parseAnd<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseForAll<ast::Precondition>(context, astContext, variableStack, parsePrecondition)))
	{
		return std::move(precondition.value());
	}

	tokenizer.seek(position);
	return parsePreconditionBody(context, astContext, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::Precondition> parsePreconditionBody(Context &context, ASTContext &astContext, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.skipWhiteSpace();

	// Test unsupported expressions first
	const auto position = tokenizer.position();

	tokenizer.expect<std::string>("(");
	tokenizer.skipWhiteSpace();

	const auto expressionIdentifierPosition = tokenizer.position();

	if (tokenizer.testIdentifierAndReturn("-")
		|| tokenizer.testIdentifierAndReturn("*")
		|| tokenizer.testIdentifierAndReturn("+")
		|| tokenizer.testIdentifierAndReturn("-")
		|| tokenizer.testIdentifierAndReturn("/")
		|| tokenizer.testIdentifierAndReturn(">")
		|| tokenizer.testIdentifierAndReturn("<")
		|| tokenizer.testIdentifierAndReturn(">=")
		|| tokenizer.testIdentifierAndReturn("<="))
	{
		throw exceptUnsupportedExpression(position, context);
	}

	tokenizer.seek(position);

	// Now, test supported expressions
	std::experimental::optional<ast::Precondition> precondition;

	if ((precondition = parseAnd<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseOr<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseExists<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseForAll<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
		|| (precondition = parseNot<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseImply<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseAtomicFormula(context, astContext, variableStack)))
	{
		return std::move(precondition.value());
	}

	tokenizer.seek(expressionIdentifierPosition);
	const auto expressionIdentifier = tokenizer.getIdentifier();

	tokenizer.seek(position);
	throw ParserException(tokenizer.location(), "expression type “" + expressionIdentifier + "” unknown or not allowed in precondition body");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
