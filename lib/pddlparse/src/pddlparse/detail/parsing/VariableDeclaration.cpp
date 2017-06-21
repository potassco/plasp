#include <pddlparse/detail/parsing/VariableDeclaration.h>

#include <pddlparse/AST.h>
#include <pddlparse/Exception.h>
#include <pddlparse/detail/ASTCopy.h>
#include <pddlparse/detail/parsing/Type.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VariableDeclaration
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddUntypedVariableDeclaration(Context &context, ast::VariableDeclarations &variableDeclarations)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.expect<std::string>("?");
	auto variableName = tokenizer.getIdentifier();
	assert(variableName != "-");

	variableDeclarations.emplace_back(std::make_unique<ast::VariableDeclaration>(std::move(variableName)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddVariableDeclarations(Context &context, ast::Domain &domain, ast::VariableDeclarations &variableDeclarations)
{
	auto &tokenizer = context.tokenizer;
	tokenizer.skipWhiteSpace();

	// Index on the first element of the current inheritance list
	size_t inheritanceIndex = variableDeclarations.size();

	while (tokenizer.currentCharacter() != ')')
	{
		parseAndAddUntypedVariableDeclaration(context, variableDeclarations);

		tokenizer.skipWhiteSpace();

		if (!tokenizer.testAndSkip<char>('-'))
			continue;

		auto parentType = parseType(context, domain);

		for (size_t i = inheritanceIndex; i < variableDeclarations.size(); i++)
			variableDeclarations[i]->type = ast::deepCopy(parentType);

		// All types up to now are labeled with their parent types
		inheritanceIndex = variableDeclarations.size();

		tokenizer.skipWhiteSpace();
	}

	const bool isTypingUsed = !domain.types.empty();

	if (isTypingUsed && !variableDeclarations.empty() && !variableDeclarations.back()->type)
		throw ParserException(tokenizer.location(), "missing type declaration for variable “?" + variableDeclarations.back()->name + "”");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::VariableDeclarations parseVariableDeclarations(Context &context, ast::Domain &domain)
{
	ast::VariableDeclarations result;

	parseAndAddVariableDeclarations(context, domain, result);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
