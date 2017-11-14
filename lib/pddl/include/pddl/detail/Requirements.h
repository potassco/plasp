#ifndef __PDDL__DETAIL__REQUIREMENTS_H
#define __PDDL__DETAIL__REQUIREMENTS_H

#include <pddl/Context.h>
#include <pddl/detail/ASTContext.h>

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
