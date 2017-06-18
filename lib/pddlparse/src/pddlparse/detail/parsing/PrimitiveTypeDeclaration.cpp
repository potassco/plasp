#include <pddlparse/detail/parsing/PrimitiveTypeDeclaration.h>

#include <pddlparse/Exception.h>
#include <pddlparse/detail/ASTCopy.h>
#include <pddlparse/detail/parsing/PrimitiveType.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PrimitiveTypeDeclaration
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ast::PrimitiveTypeDeclarationPointer &parseAndAddUntypedPrimitiveTypeDeclaration(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;
	auto typeName = tokenizer.getIdentifier();

	auto &types = domain.types;

	const auto matchingPrimitiveType = std::find_if(types.begin(), types.end(),
		[&](const auto &primitiveType)
		{
			return primitiveType->name == typeName;
		});

	// Return existing primitive type
	if (matchingPrimitiveType != types.cend())
		return *matchingPrimitiveType;

	types.emplace_back(std::make_unique<ast::PrimitiveTypeDeclaration>(std::move(typeName)));

	return types.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddPrimitiveTypeDeclarations(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;
	tokenizer.skipWhiteSpace();

	const auto position = tokenizer.position();
	const auto typeStartIndex = domain.types.size();

	// First pass: collect all primitive types
	while (tokenizer.currentCharacter() != ')')
	{
		parseAndAddUntypedPrimitiveTypeDeclaration(context, domain);

		tokenizer.skipWhiteSpace();

		if (!tokenizer.testAndSkip<char>('-'))
			continue;

		// Skip parent type information for now
		tokenizer.getIdentifier();
		tokenizer.skipWhiteSpace();
	}

	tokenizer.seek(position);

	// Second pass: link parent types correctly
	// Index on the first element of the current inheritance list
	// TODO: test correct implementation of offset if this function is called multiple times
	size_t inheritanceIndex = typeStartIndex;
	size_t i = typeStartIndex;

	while (tokenizer.currentCharacter() != ')')
	{
		// Skip type declaration
		tokenizer.getIdentifier();
		tokenizer.skipWhiteSpace();

		if (!tokenizer.testAndSkip<char>('-'))
		{
			i++;
			continue;
		}

		// If existing, parse and store parent type
		auto parentType = parsePrimitiveType(context, domain);
		tokenizer.skipWhiteSpace();

		auto &types = domain.types;

		for (size_t j = inheritanceIndex; j <= i; j++)
			types[j]->parentTypes.emplace_back(ast::deepCopy(parentType));

		// All types up to now are labeled with their parent types
		inheritanceIndex = i + 1;
		i++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
