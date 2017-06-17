#ifndef __PDDL_PARSE__DETAIL__PARSING__PREDICATE_H
#define __PDDL_PARSE__DETAIL__PARSING__PREDICATE_H

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
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: rename consistently
std::experimental::optional<ast::PredicatePointer> parsePredicate(Context &context, ASTContext &astContext, VariableStack &variableStack);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
