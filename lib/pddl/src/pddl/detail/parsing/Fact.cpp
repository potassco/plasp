#include <pddl/detail/parsing/Fact.h>

#include <pddl/AST.h>
#include <pddl/detail/parsing/AtomicFormula.h>
#include <pddl/detail/parsing/Expressions.h>
#include <pddl/detail/parsing/Unsupported.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Fact
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::Fact> parseFact(Context &context, ASTContext &astContext, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	// Test unsupported expressions first
	const auto position = tokenizer.position();

	tokenizer.expect<std::string>("(");
	tokenizer.skipWhiteSpace();

	if (tokenizer.testIdentifierAndReturn("="))
		throw exceptUnsupportedExpression(position, context);

	tokenizer.seek(position);

	// Now, test supported expressions
	std::experimental::optional<ast::Fact> fact;

	if ((fact = parseNot<ast::AtomicFormula>(context, astContext, variableStack, parseAtomicFormula))
		|| (fact = parseAtomicFormula(context, astContext, variableStack)))
	{
		return std::move(fact.value());
	}

	// Test for “at” expressions only now to allow “at” as a predicate name
	// TODO: allow this in compatibility mode only?
	if (tokenizer.testIdentifierAndReturn("at"))
		throw exceptUnsupportedExpression(position, context);

	return std::experimental::nullopt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
