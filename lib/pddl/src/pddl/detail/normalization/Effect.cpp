#include <pddl/detail/normalization/Effect.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/normalization/ConditionalEffect.h>
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

normalizedAST::Effect normalize(ast::Effect &&effect, detail::NormalizationContext &normalizationContext)
{
	const auto handleLiteral =
		[](ast::Literal &literal) -> normalizedAST::Effect
		{
			return normalize(std::move(literal));
		};

	const auto handleAnd =
		[&](ast::AndPointer<ast::Effect> &and_) -> normalizedAST::Effect
		{
			normalizedAST::And<normalizedAST::Effect>::Arguments arguments;

			for (auto &argument : and_->arguments)
				arguments.emplace_back(normalize(std::move(argument), normalizationContext));

			return std::make_unique<normalizedAST::And<normalizedAST::Effect>>(std::move(arguments));
		};

	const auto handleForAll =
		[&](ast::ForAllPointer<ast::Effect> &forAll) -> normalizedAST::Effect
		{
			auto normalizedArgument = normalize(std::move(forAll->argument), normalizationContext);
			return std::make_unique<normalizedAST::ForAll<normalizedAST::Effect>>(std::move(forAll->parameters), std::move(normalizedArgument));
		};

	const auto handleWhen =
		[&](ast::WhenPointer<ast::Precondition, ast::ConditionalEffect> &when) -> normalizedAST::Effect
		{
			auto normalizedCondition = normalize(std::move(when->argumentLeft), normalizationContext);
			auto normalizedConditionalEffect = normalize(std::move(when->argumentRight));

			return std::make_unique<normalizedAST::When<normalizedAST::Precondition, normalizedAST::ConditionalEffect>>(std::move(normalizedCondition), std::move(normalizedConditionalEffect));
		};

	return effect.match(handleLiteral, handleAnd, handleForAll, handleWhen);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
