#include <pddl/detail/parsing/ConstantDeclaration.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/detail/ASTCopy.h>
#include <pddl/detail/parsing/PrimitiveType.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ConstantDeclaration
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddUntypedConstantDeclaration(Context &context, ast::ConstantDeclarations &constantDeclarations)
{
	auto &tokenizer = context.tokenizer;

	auto constantName = tokenizer.getIdentifier();
	assert(constantName != "-");

	constantDeclarations.emplace_back(std::make_unique<ast::ConstantDeclaration>(std::move(constantName)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddConstantDeclarations(Context &context, ast::Domain &domain, ast::ConstantDeclarations &constantDeclarations)
{
	auto &tokenizer = context.tokenizer;
	tokenizer.skipWhiteSpace();

	// Index on the first element of the current inheritance list
	size_t inheritanceIndex = constantDeclarations.size();

	while (tokenizer.currentCharacter() != ')')
	{
		parseAndAddUntypedConstantDeclaration(context, constantDeclarations);

		tokenizer.skipWhiteSpace();

		if (!tokenizer.testAndSkip<char>('-'))
			continue;

		// If existing, parse and store parent type
		auto parentType = parsePrimitiveType(context, domain);

		for (size_t i = inheritanceIndex; i < constantDeclarations.size(); i++)
			constantDeclarations[i]->type = ast::deepCopy(parentType);

		// All types up to now are labeled with their parent types
		inheritanceIndex = constantDeclarations.size();

		tokenizer.skipWhiteSpace();
	}

	const bool isTypingUsed = !domain.types.empty();

	if (isTypingUsed && !constantDeclarations.empty() && !constantDeclarations.back()->type)
		throw ParserException(tokenizer.location(), "missing type declaration for constant “" + constantDeclarations.back()->name + "”");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddConstantDeclarations(Context &context, ast::Domain &domain)
{
	parseAndAddConstantDeclarations(context, domain, domain.constants);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddConstantDeclarations(Context &context, ast::Problem &problem)
{
	parseAndAddConstantDeclarations(context, *problem.domain, problem.objects);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
