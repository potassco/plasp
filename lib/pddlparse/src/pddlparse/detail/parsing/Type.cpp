#include <pddlparse/detail/parsing/Type.h>

#include <pddlparse/ParserException.h>
#include <pddlparse/detail/parsing/Expressions.h>
#include <pddlparse/detail/parsing/PrimitiveType.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Type
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ast::Type parseType(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.skipWhiteSpace();

	if (tokenizer.testAndReturn<char>('('))
	{
		// TODO: put into Type parsing unit
		// TODO: refactor
		auto p =
			[](auto &context, auto &astContext, auto &) -> std::experimental::optional<ast::PrimitiveTypePointer>
			{
				return parsePrimitiveType(context, *astContext.domain);
			};

		// TODO: refactor
		ASTContext astContext(domain);
		VariableStack variableStack;

		auto eitherType = parseEither<ast::PrimitiveTypePointer>(context, astContext, variableStack, p);

		if (!eitherType)
			throw ParserException(tokenizer.location(), "expected primitive type or “either” expression");

		return std::move(eitherType.value());
	}

	// If existing, parse and store parent type
	return parsePrimitiveType(context, domain);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
