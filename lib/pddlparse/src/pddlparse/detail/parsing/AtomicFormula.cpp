#include <pddlparse/detail/parsing/AtomicFormula.h>

#include <pddlparse/AST.h>
#include <pddlparse/detail/parsing/Predicate.h>
#include <pddlparse/detail/parsing/Unsupported.h>

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
	{
		tokenizer.seek(position);
		return parseUnsupported(context);
	}

	tokenizer.seek(position);

	// Now, test supported expressions
	std::experimental::optional<ast::AtomicFormula> atomicFormula;

	if ((atomicFormula = parsePredicate(context, astContext, variableStack)))
		return std::move(atomicFormula.value());

	tokenizer.seek(position);
	return std::experimental::nullopt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
