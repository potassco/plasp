#ifndef __PDDL__DETAIL__PARSING__VARIABLE_DECLARATION_H
#define __PDDL__DETAIL__PARSING__VARIABLE_DECLARATION_H

#include <pddl/ASTForward.h>
#include <pddl/Context.h>

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
