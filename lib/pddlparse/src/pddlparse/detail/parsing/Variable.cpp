#include <pddlparse/detail/parsing/Variable.h>

#include <pddlparse/AST.h>
#include <pddlparse/ParserException.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ast::VariablePointer parseVariable(Context &context, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.expect<std::string>("?");

	const auto variableName = tokenizer.getIdentifier();

	auto variableDeclaration = variableStack.findVariableDeclaration(variableName);

	if (!variableDeclaration)
		throw ParserException(tokenizer.location(), "undeclared variable “" + variableName + "”");

	return std::make_unique<ast::Variable>(variableDeclaration.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
