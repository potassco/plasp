#ifndef __PDDL_PARSE__DETAIL__PARSING__AST_H
#define __PDDL_PARSE__DETAIL__PARSING__AST_H

#include <pddlparse/AST.h>
#include <pddlparse/detail/parsing/Parser.h>
#include <pddlparse/detail/parsing/Utils.h>

namespace pddl
{
namespace detail
{
/*
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ParseAST
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitives
////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
struct Parser<ast::Constant>
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
struct Parser<ast::ConstantDeclaration>
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
struct Parser<ast::PrimitiveType>
{
	std::experimental::optional<ast::PrimitiveType> parse(Context &context, ASTContext &astContext)
	{
		auto &tokenizer = context.tokenizer;
		auto &types = astContext.description.domain.types;

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

				return ast::PrimitiveType(types.back());
			}
			else
				throw tokenize::TokenizerException(tokenizer.location(), "type “" + typeName + "” used but never declared");
		}

		auto &type = *matchingType;

		return ast::PrimitiveType(type);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
struct Parser<ast::PrimitiveTypeDeclaration>
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
struct Parser<ast::Unsupported>
{
	std::experimental::optional<ast::Unsupported> parse(Context &context, ASTContext &)
	{
		auto &tokenizer = context.tokenizer;

		ast::Unsupported unsupported;

		tokenizer.expect<std::string>("(");

		unsupported.type = tokenizer.getIdentifier();

		context.warningCallback(tokenizer.location(), "expression type “" + unsupported.type + "” currently unsupported in this context");

		skipSection(tokenizer);

		return unsupported;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
struct Parser<ast::And<Argument>>
{
	template<typename ArgumentParser>
	std::experimental::optional<ast::And<Argument>> parse(Context &context, ASTContext &astContext, ArgumentParser parseArgument)
	{
		return Parser<ast::NAry<ast::And<Argument>, Argument>>::parse(context, astContext, parseArgument);
	}
};
*/
////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
