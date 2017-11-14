#include <pddl/detail/normalization/AtomicFormula.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/NormalizedAST.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AtomicFormula
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::AtomicFormula normalize(ast::AtomicFormula &&atomicFormula)
{
	const auto handleEquals =
		[&](ast::EqualsPointer<ast::Term, ast::Term> &) -> normalizedAST::AtomicFormula
		{
			// TODO: implement
			throw NormalizationException("“=” expressions currently unsupported by normalization");
		};

	const auto handlePredicate =
		[&](ast::PredicatePointer &predicate) -> normalizedAST::AtomicFormula
		{
			return std::move(predicate);
		};

	return atomicFormula.match(handleEquals, handlePredicate);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
