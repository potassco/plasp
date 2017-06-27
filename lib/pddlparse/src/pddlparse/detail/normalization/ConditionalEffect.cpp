#include <pddlparse/detail/normalization/Effect.h>

#include <pddlparse/AST.h>
#include <pddlparse/Exception.h>
#include <pddlparse/NormalizedAST.h>
#include <pddlparse/detail/normalization/Literal.h>
#include <pddlparse/detail/normalization/Precondition.h>

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
