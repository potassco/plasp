#ifndef __PDDL__DETAIL__NORMALIZATION__INITIAL_STATE_H
#define __PDDL__DETAIL__NORMALIZATION__INITIAL_STATE_H

#include <pddl/ASTForward.h>
#include <pddl/Context.h>
#include <pddl/NormalizedASTForward.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InitialState
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::InitialState normalize(ast::InitialState &&initialState);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif