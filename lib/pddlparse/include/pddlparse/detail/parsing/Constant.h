#ifndef __PDDL_PARSE__DETAIL__PARSING__CONSTANT_H
#define __PDDL_PARSE__DETAIL__PARSING__CONSTANT_H

#include <pddlparse/ASTForward.h>
#include <pddlparse/Context.h>
#include <pddlparse/detail/ASTContext.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constant
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::ConstantPointer> testParsingConstant(Context &context, ASTContext &astContext);
ast::ConstantPointer parseConstant(Context &context, ASTContext &astContext);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
