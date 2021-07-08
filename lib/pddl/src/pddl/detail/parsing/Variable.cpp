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

std::optional<ast::VariablePointer> parseVariable(Context &context, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	if (!tokenizer.testAndReturn<std::string>("?"))
		return std::nullopt;

	tokenizer.expect<std::string>("?");

	auto variableName = tokenizer.getIdentifier();
	auto variableDeclaration = variableStack.findVariableDeclaration(variableName);

	if (!variableDeclaration)
		return std::nullopt;

	return std::make_unique<ast::Variable>(variableDeclaration.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
