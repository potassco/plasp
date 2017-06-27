#include <pddlparse/detail/normalization/Effect.h>

#include <pddlparse/AST.h>
#include <pddlparse/Exception.h>
#include <pddlparse/NormalizedAST.h>
#include <pddlparse/detail/normalization/ConditionalEffect.h>
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

normalizedAST::Effect normalize(ast::Effect &&effect, normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
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
				arguments.emplace_back(normalize(std::move(argument), derivedPredicates));

			return std::make_unique<normalizedAST::And<normalizedAST::Effect>>(std::move(arguments));
		};

	const auto handleForAll =
		[&](ast::ForAllPointer<ast::Effect> &forAll) -> normalizedAST::Effect
		{
			auto normalizedArgument = normalize(std::move(forAll->argument), derivedPredicates);
			return std::make_unique<normalizedAST::ForAll<normalizedAST::Effect>>(std::move(forAll->parameters), std::move(normalizedArgument));
		};

	const auto handleWhen =
		[&](ast::WhenPointer<ast::Precondition, ast::ConditionalEffect> &when) -> normalizedAST::Effect
		{
			auto normalizedCondition = normalize(std::move(when->argumentLeft), derivedPredicates);
			auto normalizedConditionalEffect = normalize(std::move(when->argumentRight));

			return std::make_unique<normalizedAST::When<normalizedAST::Precondition, normalizedAST::ConditionalEffect>>(std::move(normalizedCondition), std::move(normalizedConditionalEffect));
		};

	return effect.match(handleLiteral, handleAnd, handleForAll, handleWhen);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
