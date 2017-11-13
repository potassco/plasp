#include <pddl/detail/parsing/Type.h>

#include <pddl/Exception.h>
#include <pddl/detail/parsing/Expressions.h>
#include <pddl/detail/parsing/PrimitiveType.h>

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
		// TODO: refactor
		auto parsePrimitiveTypeWrapper =
			[](auto &context, auto &astContext, auto &) -> std::experimental::optional<ast::PrimitiveTypePointer>
			{
				return parsePrimitiveType(context, *astContext.domain);
			};

		// TODO: refactor
		ASTContext astContext(domain);
		VariableStack variableStack;

		auto eitherType = parseEither<ast::PrimitiveTypePointer>(context, astContext, variableStack, parsePrimitiveTypeWrapper);

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
