#ifndef __PDDL_PARSE__DETAIL__PARSING__ATOMIC_FORMULA_H
#define __PDDL_PARSE__DETAIL__PARSING__ATOMIC_FORMULA_H

#include <pddlparse/ASTForward.h>
#include <pddlparse/Context.h>
#include <pddlparse/detail/ASTContext.h>
#include <pddlparse/detail/VariableStack.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AtomicFormula
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::AtomicFormula> parseAtomicFormula(Context &context, ASTContext &astContext, VariableStack &variableStack);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
