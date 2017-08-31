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

	// TODO: refactor
	auto variable = parseVariable(context, variableStack);

	if (variable)
		return std::move(variable.value());

	// Parse argument if it is a constant
	auto constant = testParsingConstant(context, astContext);

	if (constant)
		return std::move(constant.value());

	return std::experimental::nullopt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
