#include <pddl/detail/parsing/Term.h>

#include <pddl/Exception.h>
#include <pddl/detail/parsing/Constant.h>
#include <pddl/detail/parsing/Variable.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Term
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::Term> parseTerm(Context &context, ASTContext &astContext, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.skipWhiteSpace();

	std::experimental::optional<ast::Term> term;

	if ((term = parseVariable(context, variableStack))
	    || (term = parseConstant(context, astContext)))
	{
		return std::move(term.value());
	}

	return std::experimental::nullopt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
