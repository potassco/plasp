#include <pddlparse/detail/normalization/Reduction.h>

#include <pddlparse/AST.h>
#include <pddlparse/Exception.h>
#include <pddlparse/NormalizedAST.h>
#include <pddlparse/detail/normalization/AtomicFormula.h>
#include <pddlparse/detail/normalization/CollectFreeVariables.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Reduction
//
////////////////////////////////////////////////////////////////////////////////////////////////////

const auto handleUnsupported =
	[](ast::UnsupportedPointer &unsupported)
	{
		throw NormalizationException("“" + unsupported->type + "” expressions in preconditions can’t be normalized currently");
	};

////////////////////////////////////////////////////////////////////////////////////////////////////

void eliminateImply(ast::Precondition &precondition)
{
	const auto handleAtomicFormula =
		[](ast::AtomicFormula &)
		{
		};

	const auto handleAnd =
		[](ast::AndPointer<ast::Precondition> &and_)
		{
			for (auto &argument : and_->arguments)
				eliminateImply(argument);
		};

	const auto handleExists =
		[](ast::ExistsPointer<ast::Precondition> &exists)
		{
			eliminateImply(exists->argument);
		};

	const auto handleForAll =
		[](ast::ForAllPointer<ast::Precondition> &forAll)
		{
			eliminateImply(forAll->argument);
		};

	const auto handleImply =
		[&](ast::ImplyPointer<ast::Precondition> &imply)
		{
			eliminateImply(imply->argumentLeft);
			eliminateImply(imply->argumentRight);

			ast::Or<ast::Precondition>::Arguments arguments;
			arguments.reserve(2);
			arguments.emplace_back(std::make_unique<ast::Not<ast::Precondition>>(std::move(imply->argumentLeft)));
			arguments.emplace_back(std::move(imply->argumentRight));

			precondition = std::make_unique<ast::Or<ast::Precondition>>(std::move(arguments));
		};

	const auto handleNot =
		[](ast::NotPointer<ast::Precondition> &not_)
		{
			eliminateImply(not_->argument);
		};

	const auto handleOr =
		[](ast::OrPointer<ast::Precondition> &or_)
		{
			for (auto &argument : or_->arguments)
				eliminateImply(argument);
		};

	precondition.match(handleAtomicFormula, handleAnd, handleExists, handleForAll, handleImply, handleNot, handleOr, handleUnsupported);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void reduce(ast::Precondition &precondition)
{
	// Replace “imply” statements with disjunctions
	eliminateImply(precondition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
