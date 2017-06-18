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

std::experimental::optional<ast::PrimitiveTypeDeclarationPointer *> findPrimitiveTypeDeclaration(ast::Domain &domain, const std::string &typeName)
{
	auto &types = domain.types;

	const auto matchingPrimitiveType = std::find_if(types.begin(), types.end(),
		[&](const auto &primitiveType)
		{
			return primitiveType->name == typeName;
		});

	if (matchingPrimitiveType != types.end())
		return &*matchingPrimitiveType;

	return std::experimental::nullopt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::PrimitiveTypeDeclarationPointer &parseAndAddUntypedPrimitiveTypeDeclaration(Context &context, ast::Domain &domain, std::vector<bool> &flaggedTypes)
{
	auto &tokenizer = context.tokenizer;
	auto typeName = tokenizer.getIdentifier();

	auto &types = domain.types;

	auto matchingPrimitiveTypeDeclaration = findPrimitiveTypeDeclaration(domain, typeName);

	if (matchingPrimitiveTypeDeclaration)
		return *matchingPrimitiveTypeDeclaration.value();

	types.emplace_back(std::make_unique<ast::PrimitiveTypeDeclaration>(std::move(typeName)));
	flaggedTypes.emplace_back(false);

	return types.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddPrimitiveTypeDeclarations(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;
	tokenizer.skipWhiteSpace();

	auto &types = domain.types;

	std::vector<bool> flaggedTypes;
	flaggedTypes.resize(types.size(), false);

	while (tokenizer.currentCharacter() != ')')
	{
		auto &childType = parseAndAddUntypedPrimitiveTypeDeclaration(context, domain, flaggedTypes);
		flaggedTypes[&childType - &types.front()] = true;

		tokenizer.skipWhiteSpace();

		if (!tokenizer.testAndSkip<char>('-'))
			continue;

		// Skip parent type information for now
		auto &parentType = parseAndAddUntypedPrimitiveTypeDeclaration(context, domain, flaggedTypes);

		for (size_t i = 0; i < flaggedTypes.size(); i++)
			if (flaggedTypes[i])
			{
				flaggedTypes[i] = false;
				types[i]->parentTypes.emplace_back(std::make_unique<ast::PrimitiveType>(parentType.get()));
			}

		tokenizer.skipWhiteSpace();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
