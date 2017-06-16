#include <pddlparse/detail/parsing/Precondition.h>

#include <pddlparse/AST.h>
#include <pddlparse/detail/parsing/Expressions.h>
#include <pddlparse/detail/parsing/Predicate.h>
#include <pddlparse/detail/parsing/Unsupported.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Precondition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::Precondition> parsePreconditionBody(Context &context, ASTContext &astContext, VariableStack &variableStack);

////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::Precondition> parsePrecondition(Context &context, ASTContext &astContext, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.skipWhiteSpace();

	std::experimental::optional<ast::Precondition> precondition;

	if ((precondition = parseAnd<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseForAll<ast::Precondition>(context, astContext, variableStack, parsePrecondition)))
	{
		return std::move(precondition.value());
	}

	const auto position = tokenizer.position();

	tokenizer.expect<std::string>("(");

	const auto expressionIdentifierPosition = tokenizer.position();

	if (tokenizer.testIdentifierAndSkip("preference"))
	{
		// TODO: refactor
		tokenizer.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = tokenizer.getIdentifier();

		tokenizer.seek(position);
		return parseUnsupported(context);
	}

	tokenizer.seek(position);
	return parsePreconditionBody(context, astContext, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::Precondition> parsePreconditionBody(Context &context, ASTContext &astContext, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.skipWhiteSpace();

	std::experimental::optional<ast::Precondition> precondition;

	if ((precondition = parseAnd<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseOr<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseExists<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseForAll<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
		|| (precondition = parseNot<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parseImply<ast::Precondition>(context, astContext, variableStack, parsePrecondition))
	    || (precondition = parsePredicate(context, astContext, variableStack)))
	{
		return std::move(precondition.value());
	}

	const auto position = tokenizer.position();

	tokenizer.expect<std::string>("(");

	const auto expressionIdentifierPosition = tokenizer.position();

	if (tokenizer.testIdentifierAndSkip("-")
		|| tokenizer.testIdentifierAndSkip("=")
		|| tokenizer.testIdentifierAndSkip("*")
		|| tokenizer.testIdentifierAndSkip("+")
		|| tokenizer.testIdentifierAndSkip("-")
		|| tokenizer.testIdentifierAndSkip("/")
		|| tokenizer.testIdentifierAndSkip(">")
		|| tokenizer.testIdentifierAndSkip("<")
		|| tokenizer.testIdentifierAndSkip("=")
		|| tokenizer.testIdentifierAndSkip(">=")
		|| tokenizer.testIdentifierAndSkip("<="))
	{
		tokenizer.seek(expressionIdentifierPosition);
		const auto expressionIdentifier = tokenizer.getIdentifier();

		tokenizer.seek(position);
		return parseUnsupported(context);
	}

	tokenizer.seek(expressionIdentifierPosition);
	const auto expressionIdentifier = tokenizer.getIdentifier();

	tokenizer.seek(position);
	throw tokenize::TokenizerException(tokenizer.location(), "expression type “" + expressionIdentifier + "” unknown or not allowed in this context");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
