#include <pddlparse/detail/normalization/AtomicFormula.h>

#include <pddlparse/AST.h>
#include <pddlparse/Exception.h>
#include <pddlparse/NormalizedAST.h>

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
	const auto handlePredicate =
		[&](ast::PredicatePointer &predicate) -> normalizedAST::AtomicFormula
		{
			return std::move(predicate);
		};

	return atomicFormula.match(handlePredicate);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
