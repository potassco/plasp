#include <pddl/detail/parsing/Variable.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<std::string> testParsingVariableName(Context &context)
{
	auto &tokenizer = context.tokenizer;

	if (!tokenizer.testAndReturn<std::string>("?"))
		return std::experimental::nullopt;

	tokenizer.expect<std::string>("?");

	return tokenizer.getIdentifier();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string parseVariableName(Context &context)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.expect<std::string>("?");

	return tokenizer.getIdentifier();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::VariablePointer> testParsingVariable(Context &context, VariableStack &variableStack)
{
	auto variableName = testParsingVariableName(context);

	if (!variableName)
		return std::experimental::nullopt;

	auto variableDeclaration = variableStack.findVariableDeclaration(variableName.value());

	if (!variableDeclaration)
		return std::experimental::nullopt;

	return std::make_unique<ast::Variable>(variableDeclaration.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::VariablePointer parseVariable(Context &context, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	auto variableName = parseVariableName(context);
	auto variableDeclaration = variableStack.findVariableDeclaration(variableName);

	if (!variableDeclaration)
		throw ParserException(tokenizer.location(), "undeclared variable “" + variableName + "”");

	return std::make_unique<ast::Variable>(variableDeclaration.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
