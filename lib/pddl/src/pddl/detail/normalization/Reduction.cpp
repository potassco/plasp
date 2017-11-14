#include <pddl/detail/normalization/Reduction.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/normalization/AtomicFormula.h>
#include <pddl/detail/normalization/CollectFreeVariables.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Reduction
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////////////////////////////////////////////

void eliminateImply(ast::Precondition &precondition);
void negationNormalize(ast::Precondition &precondition);
void eliminateForAll(ast::Precondition &precondition);
void eliminateDoubleNegations(ast::Precondition &precondition);

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

	precondition.match(handleAtomicFormula, handleAnd, handleExists, handleForAll, handleImply, handleNot, handleOr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Negation-normalize an expression whose parent is a “not” expression
void negationNormalizeNegated(ast::Precondition &precondition, ast::Precondition &parent)
{
	const auto handleAtomicFormula =
		[](ast::AtomicFormula &)
		{
		};

	const auto handleAnd =
		[&](ast::AndPointer<ast::Precondition> &and_)
		{
			ast::Or<ast::Precondition>::Arguments arguments;
			arguments.reserve(and_->arguments.size());

			// Apply De Morgan
			for (auto &argument : and_->arguments)
				arguments.emplace_back(std::make_unique<ast::Not<ast::Precondition>>(std::move(argument)));

			// Finally, negation-normalize each argument
			for (auto &argument : arguments)
				negationNormalize(argument);

			// Replace the parent “not” containing this “and” with an “or” over the negated arguments
			parent = std::make_unique<ast::Or<ast::Precondition>>(std::move(arguments));
		};

	const auto handleExists =
		[](ast::ExistsPointer<ast::Precondition> &exists)
		{
			negationNormalize(exists->argument);
		};

	const auto handleForAll =
		[](ast::ForAllPointer<ast::Precondition> &forAll)
		{
			negationNormalize(forAll->argument);
		};

	const auto handleImply =
		[](ast::ImplyPointer<ast::Precondition> &)
		{
			throw std::logic_error("precondition not ready for negation normalization (imply), please report to the bug tracker");
		};

	const auto handleNot =
		[&](ast::NotPointer<ast::Precondition> &not_)
		{
			negationNormalize(not_->argument);

			// As the parent contains the argument, the argument needs to be saved before overwriting the parent
			// TODO: check whether this workaround can be avoided
			auto argument = std::move(not_->argument);
			parent = std::move(argument);
		};

	const auto handleOr =
		[&](ast::OrPointer<ast::Precondition> &or_)
		{
			ast::And<ast::Precondition>::Arguments arguments;
			arguments.reserve(or_->arguments.size());

			// Apply De Morgan
			for (auto &argument : or_->arguments)
				arguments.emplace_back(std::make_unique<ast::Not<ast::Precondition>>(std::move(argument)));

			// Finally, negation-normalize each argument
			for (auto &argument : arguments)
				negationNormalize(argument);

			// Replace the parent “not” containing this “or” with an “and” over the negated arguments
			parent = std::make_unique<ast::And<ast::Precondition>>(std::move(arguments));
		};

	precondition.match(handleAtomicFormula, handleAnd, handleExists, handleForAll, handleImply, handleNot, handleOr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void negationNormalize(ast::Precondition &precondition)
{
	const auto handleAtomicFormula =
		[](ast::AtomicFormula &)
		{
		};

	const auto handleAnd =
		[](ast::AndPointer<ast::Precondition> &and_)
		{
			for (auto &argument : and_->arguments)
				negationNormalize(argument);
		};

	const auto handleExists =
		[](ast::ExistsPointer<ast::Precondition> &exists)
		{
			negationNormalize(exists->argument);
		};

	const auto handleForAll =
		[](ast::ForAllPointer<ast::Precondition> &forAll)
		{
			negationNormalize(forAll->argument);
		};

	const auto handleImply =
		[](ast::ImplyPointer<ast::Precondition> &)
		{
			throw std::logic_error("precondition not ready for negation normalization (imply), please report to the bug tracker");
		};

	const auto handleNot =
		[&](ast::NotPointer<ast::Precondition> &not_)
		{
			negationNormalizeNegated(not_->argument, precondition);
		};

	const auto handleOr =
		[](ast::OrPointer<ast::Precondition> &or_)
		{
			for (auto &argument : or_->arguments)
				negationNormalize(argument);
		};

	precondition.match(handleAtomicFormula, handleAnd, handleExists, handleForAll, handleImply, handleNot, handleOr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void eliminateForAll(ast::Precondition &precondition)
{
	const auto handleAtomicFormula =
		[](ast::AtomicFormula &)
		{
		};

	const auto handleAnd =
		[](ast::AndPointer<ast::Precondition> &and_)
		{
			for (auto &argument : and_->arguments)
				eliminateForAll(argument);
		};

	const auto handleExists =
		[](ast::ExistsPointer<ast::Precondition> &exists)
		{
			eliminateForAll(exists->argument);
		};

	const auto handleForAll =
		[&](ast::ForAllPointer<ast::Precondition> &forAll)
		{
			eliminateForAll(forAll->argument);

			auto negatedArgument = std::make_unique<ast::Not<ast::Precondition>>(std::move(forAll->argument));
			auto exists = std::make_unique<ast::Exists<ast::Precondition>>(std::move(forAll->parameters), std::move(negatedArgument));
			precondition = std::make_unique<ast::Not<ast::Precondition>>(std::move(exists));
		};

	const auto handleImply =
		[&](ast::ImplyPointer<ast::Precondition> &imply)
		{
			eliminateForAll(imply->argumentLeft);
			eliminateForAll(imply->argumentRight);
		};

	const auto handleNot =
		[](ast::NotPointer<ast::Precondition> &not_)
		{
			eliminateForAll(not_->argument);
		};

	const auto handleOr =
		[](ast::OrPointer<ast::Precondition> &or_)
		{
			for (auto &argument : or_->arguments)
				eliminateForAll(argument);
		};

	precondition.match(handleAtomicFormula, handleAnd, handleExists, handleForAll, handleImply, handleNot, handleOr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void reduce(ast::Precondition &precondition)
{
	// Replace “imply” statements with disjunctions
	eliminateImply(precondition);

	// Eliminate “forall” statements
	eliminateForAll(precondition);

	// Negation-normalize the precondition
	negationNormalize(precondition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
