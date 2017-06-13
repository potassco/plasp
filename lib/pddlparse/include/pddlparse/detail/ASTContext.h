#ifndef __PDDL_PARSE__DETAIL__AST_CONTEXT_H
#define __PDDL_PARSE__DETAIL__AST_CONTEXT_H

#include <pddlparse/AST.h>
#include <pddlparse/detail/VariableStack.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ASTContext
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct ASTContext
{
	ASTContext(ast::Description &description)
	:	domain{description.domain.get()},
		problem{description.problem.value() ? std::experimental::optional<ast::Problem *>(description.problem.value().get()) : std::experimental::nullopt}
	{
	}

	ASTContext(ast::Domain &domain)
	:	domain{&domain}
	{
	}

	ASTContext(ast::Problem &problem)
	:	domain{problem.domain},
		problem{&problem}
	{
	}

	ast::Domain *domain;
	std::experimental::optional<ast::Problem *> problem;

	VariableStack variables;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
