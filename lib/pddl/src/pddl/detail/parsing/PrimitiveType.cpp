#include <pddl/detail/parsing/PrimitiveType.h>

#include <pddl/Exception.h>
#include <pddl/detail/Requirements.h>

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

	auto typeName = tokenizer.getIdentifier();

	if (typeName.empty())
		throw ParserException(tokenizer.location(), "could not parse primitive type, expected identifier");

	auto matchingType = std::find_if(types.begin(), types.end(),
		[&](auto &primitiveTypeDeclaration)
		{
			return primitiveTypeDeclaration->name == typeName;
		});

	// If the type has not been declared yet, add it but issue a warning
	if (matchingType == types.end())
	{
		// “object” type is always allowed without warning
		if (typeName != "object")
		{
			if (context.mode != Mode::Compatibility)
				throw ParserException(tokenizer.location(), "primitive type “" + typeName + "” used without or before declaration");

			context.warningCallback(tokenizer.location(), "primitive type “" + typeName + "” used without or before declaration, silently adding declaration");
		}

		types.emplace_back(std::make_unique<ast::PrimitiveTypeDeclaration>(std::move(typeName)));

		return std::make_unique<ast::PrimitiveType>(types.back().get());
	}

	return std::make_unique<ast::PrimitiveType>(matchingType->get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
