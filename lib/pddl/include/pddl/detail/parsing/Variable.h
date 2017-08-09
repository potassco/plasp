#ifndef __PDDL__DETAIL__PARSING__VARIABLE_H
#define __PDDL__DETAIL__PARSING__VARIABLE_H

#include <pddl/ASTForward.h>
#include <pddl/Context.h>
#include <pddl/detail/VariableStack.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: find consistent naming scheme
std::experimental::optional<ast::VariablePointer> testParsingVariable(Context &context, VariableStack &variableStack);
ast::VariablePointer parseVariable(Context &context, VariableStack &variableStack);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
