#ifndef __PDDL__DETAIL__PARSING__PRECONDITION_H
#define __PDDL__DETAIL__PARSING__PRECONDITION_H

#include <pddl/ASTForward.h>
#include <pddl/Context.h>
#include <pddl/detail/ASTContext.h>
#include <pddl/detail/VariableStack.h>

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
std::experimental::optional<ast::Precondition> parsePreconditionBody(Context &context, ASTContext &astContext, VariableStack &variableStack);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
