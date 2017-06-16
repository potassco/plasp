#ifndef __PDDL_PARSE__DETAIL__PARSING__VARIABLE_DECLARATION_H
#define __PDDL_PARSE__DETAIL__PARSING__VARIABLE_DECLARATION_H

#include <pddlparse/ASTForward.h>
#include <pddlparse/Context.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VariableDeclaration
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddVariableDeclarations(Context &context, ast::Domain &domain, ast::VariableDeclarations &variableDeclarations);
ast::VariableDeclarations parseVariableDeclarations(Context &context, ast::Domain &domain);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
