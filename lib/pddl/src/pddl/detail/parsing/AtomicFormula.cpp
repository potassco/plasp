#include <pddl/detail/parsing/AtomicFormula.h>

#include <pddl/AST.h>
#include <pddl/detail/parsing/Expressions.h>
#include <pddl/detail/parsing/Predicate.h>
#include <pddl/detail/parsing/Term.h>

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
	std::experimental::optional<ast::AtomicFormula> atomicFormula;

	if ((atomicFormula = parseEquals<ast::Term, ast::Term>(context, astContext, variableStack, parseTerm, parseTerm)))
		return std::move(atomicFormula.value());

	return parsePredicate(context, astContext, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
