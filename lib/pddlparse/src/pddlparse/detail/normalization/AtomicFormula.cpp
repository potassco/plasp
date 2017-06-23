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
	const auto handleUnsupported =
		[&](ast::UnsupportedPointer &unsupported) -> normalizedAST::AtomicFormula
		{
			throw NormalizationException("“" + unsupported->type + "” expressions as literals can’t be normalized currently");
		};

	const auto handlePredicate =
		[&](ast::PredicatePointer &predicate) -> normalizedAST::AtomicFormula
		{
			return std::move(predicate);
		};

	return atomicFormula.match(handlePredicate, handleUnsupported);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
