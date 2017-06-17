#ifndef __PDDL_PARSE__DETAIL__PARSING__VARIABLE_H
#define __PDDL_PARSE__DETAIL__PARSING__VARIABLE_H

#include <pddlparse/ASTForward.h>
#include <pddlparse/Context.h>
#include <pddlparse/detail/VariableStack.h>

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
