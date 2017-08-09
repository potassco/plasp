#include <pddl/detail/normalization/Literal.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/normalization/AtomicFormula.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Literal
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Literal normalize(ast::Literal &&literal)
{
	const auto handleAtomicFormula =
		[&](ast::AtomicFormula &atomicFormula) -> normalizedAST::Literal
		{
			return normalize(std::move(atomicFormula));
		};

	const auto handleNot =
		[&](ast::NotPointer<ast::AtomicFormula> &not_) -> normalizedAST::Literal
		{
			return std::make_unique<normalizedAST::Not<normalizedAST::AtomicFormula>>(normalize(std::move(not_->argument)));
		};

	return literal.match(handleAtomicFormula, handleNot);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
