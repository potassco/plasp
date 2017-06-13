#ifndef __PDDL_PARSE__DETAIL__PARSER_H
#define __PDDL_PARSE__DETAIL__PARSER_H

#include <experimental/optional>

#include <pddlparse/ASTForward.h>
#include <pddlparse/Context.h>
#include <pddlparse/detail/ASTContext.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Parser
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
struct Parser
{
};

template<class T, typename... ArgumentParser>
std::experimental::optional<T> parse(Context &context, ASTContext &astContext, ArgumentParser... argumentParsers)
{
	return detail::Parser<T>().parse(context, astContext, argumentParsers...);
}

template<class T>
std::experimental::optional<T> parse(Context &context, ASTContext &astContext)
{
	return detail::Parser<T>().parse(context, astContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
