#ifndef __PDDL_PARSE__DETAIL__PARSING__PRECONDITION_H
#define __PDDL_PARSE__DETAIL__PARSING__PRECONDITION_H

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
// Precondition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::Precondition> parsePrecondition(Context &context, ASTContext &astContext, VariableStack &variableStack);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
