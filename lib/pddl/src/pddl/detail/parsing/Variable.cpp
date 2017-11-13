#include <pddl/detail/parsing/Variable.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::VariablePointer> parseVariable(Context &context, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	if (!tokenizer.testAndReturn<std::string>("?"))
		return std::experimental::nullopt;

	tokenizer.expect<std::string>("?");

	auto variableName = tokenizer.getIdentifier();
	auto variableDeclaration = variableStack.findVariableDeclaration(variableName);

	if (!variableDeclaration)
		return std::experimental::nullopt;

	return std::make_unique<ast::Variable>(variableDeclaration.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
