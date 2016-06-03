#ifndef __PLASP__PDDL__TYPE_H
#define __PLASP__PDDL__TYPE_H

#include <boost/variant.hpp>

#include <plasp/pddl/EitherType.h>
#include <plasp/pddl/PrimitiveType.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Type
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Context;

////////////////////////////////////////////////////////////////////////////////////////////////////

using TypePtr = boost::variant<const PrimitiveType *, const EitherType *>;

TypePtr parseType(utils::Parser &parser, Context &context);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif