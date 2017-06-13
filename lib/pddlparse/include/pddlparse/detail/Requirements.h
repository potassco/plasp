#ifndef __PDDL_PARSE__DETAIL__REQUIREMENTS_H
#define __PDDL_PARSE__DETAIL__REQUIREMENTS_H

#include <iostream>

#include <pddlparse/Context.h>
#include <pddlparse/detail/ASTContext.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Requirements
//
////////////////////////////////////////////////////////////////////////////////////////////////////

bool hasRequirement(const ast::Domain &domain, ast::Requirement requirement);
bool hasRequirement(const ast::Problem &problem, ast::Requirement requirement);
bool hasRequirement(const ASTContext &astContext, ast::Requirement requirement);

void checkRequirement(ast::Domain &domain, ast::Requirement requirement, Context &context);
void checkRequirement(ast::Problem &problem, ast::Requirement requirement, Context &context);
void checkRequirement(ASTContext &astContext, ast::Requirement requirement, Context &context);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
