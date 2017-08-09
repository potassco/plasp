#include <pddl/detail/parsing/AtomicFormula.h>

#include <pddl/AST.h>
#include <pddl/detail/parsing/Predicate.h>
#include <pddl/detail/parsing/Unsupported.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AtomicFormula
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::AtomicFormula> parseAtomicFormula(Context &context, ASTContext &astContext, VariableStack &variableStack)
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
	return parsePredicate(context, astContext, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
