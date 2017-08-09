#include <pddl/detail/normalization/Effect.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/normalization/Literal.h>
#include <pddl/detail/normalization/Precondition.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Effect
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::ConditionalEffect normalize(ast::ConditionalEffect &&conditionalEffect)
{
	const auto handleLiteral =
		[](ast::Literal &literal) -> normalizedAST::ConditionalEffect
		{
			return normalize(std::move(literal));
		};

	const auto handleAnd =
		[&](ast::AndPointer<ast::Literal> &and_) -> normalizedAST::ConditionalEffect
		{
			normalizedAST::And<normalizedAST::Literal>::Arguments arguments;

			for (auto &argument : and_->arguments)
				arguments.emplace_back(normalize(std::move(argument)));

			return std::make_unique<normalizedAST::And<normalizedAST::Literal>>(std::move(arguments));
		};

	return conditionalEffect.match(handleLiteral, handleAnd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
