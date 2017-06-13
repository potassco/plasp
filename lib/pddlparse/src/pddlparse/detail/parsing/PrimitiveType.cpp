#include <pddlparse/detail/parsing/PrimitiveType.h>

#include <pddlparse/ParserException.h>
#include <pddlparse/detail/Requirements.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PrimitiveType
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ast::PrimitiveTypePointer parsePrimitiveType(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;
	auto &types = domain.types;

	tokenizer.skipWhiteSpace();

	auto typeName = tokenizer.getIdentifier();

	if (typeName.empty())
		throw tokenize::TokenizerException(tokenizer.location(), "no type supplied");

	auto matchingType = std::find_if(types.begin(), types.end(),
		[&](auto &primitiveTypeDeclaration)
		{
			return primitiveTypeDeclaration->name == typeName;
		});

	if (matchingType == types.end())
	{
		// Only “object” is allowed as an implicit type
		if (typeName == "object" || typeName == "objects")
		{
			context.warningCallback(tokenizer.location(), "primitive type “" + typeName + "” should be declared");
			types.emplace_back(std::make_unique<ast::PrimitiveTypeDeclaration>(std::move(typeName)));

			return std::make_unique<ast::PrimitiveType>(types.back().get());
		}
		else
			throw tokenize::TokenizerException(tokenizer.location(), "type “" + typeName + "” used but never declared");
	}

	return std::make_unique<ast::PrimitiveType>(matchingType->get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
